"""Exercise session preservation with a real CP Editor process on Linux."""

import json
import os
import pathlib
import subprocess
import sys
import tempfile
import time


MARKER = "RECOVER_THIS_UNSAVED_SOURCE\nint main() { return 42; }\n"


def run_editor(binary, environment, log_path, session_path, no_restore, expect_restore, preserved_bytes=None):
    arguments = [str(binary), "--verbose"]
    if no_restore:
        arguments.append("--no-restore-session")

    with log_path.open("w", encoding="utf-8") as output:
        process = subprocess.Popen(arguments, env=environment, stdout=output, stderr=subprocess.STDOUT)
        try:
            deadline = time.monotonic() + 30
            while time.monotonic() < deadline:
                if process.poll() is not None:
                    raise RuntimeError(f"CP Editor exited during startup: {log_path.read_text(errors='replace')}")
                if "beginning the event loop" in log_path.read_text(errors="replace"):
                    break
                time.sleep(0.1)
            else:
                raise RuntimeError(f"CP Editor did not reach its event loop: {log_path.read_text(errors='replace')}")

            # The configured session auto-save interval is one second.
            time.sleep(2.5)
            if process.poll() is not None:
                raise RuntimeError(f"CP Editor exited before shutdown: {log_path.read_text(errors='replace')}")
            if not session_path.is_file():
                raise RuntimeError("The session auto-save timer did not create a session")
            if preserved_bytes is not None and session_path.read_bytes() != preserved_bytes:
                raise RuntimeError("--no-restore-session changed the saved session during auto-save")

            # CP Editor handles SIGTERM through its normal close path on Linux.
            process.terminate()
            exit_code = process.wait(timeout=15)
            if exit_code != 0:
                raise RuntimeError(f"CP Editor exited with {exit_code}: {log_path.read_text(errors='replace')}")
        finally:
            if process.poll() is None:
                process.kill()
                process.wait()

    log = log_path.read_text(errors="replace")
    if ("restoreSession" in log) != expect_restore:
        raise RuntimeError(f"Unexpected restore behavior: {log}")


def main():
    if len(sys.argv) != 2:
        raise SystemExit("usage: session_recovery.py /path/to/cpeditor")
    binary = pathlib.Path(sys.argv[1]).resolve()
    if not binary.is_file():
        raise SystemExit(f"CP Editor binary does not exist: {binary}")
    log_dir = binary.parent / "session-recovery-logs"
    log_dir.mkdir(exist_ok=True)

    with tempfile.TemporaryDirectory(prefix="cpeditor-session-recovery-") as temporary:
        root = pathlib.Path(temporary)
        config_dir = root / "config" / "cpeditor"
        config_dir.mkdir(parents=True)
        runtime_dir = root / "runtime"
        runtime_dir.mkdir(mode=0o700)

        (config_dir / "settings.ini").write_text(
            "[General]\n"
            "first_time_user=false\n"
            "promotion_dialog_shown=true\n"
            "check_update=false\n"
            "[hot_exit]\n"
            "enable=true\n"
            "auto_save=true\n"
            "auto_save_interval=1000\n",
            encoding="utf-8",
        )
        session_path = config_dir / "session.json"

        environment = os.environ.copy()
        environment.update(
            {
                "HOME": str(root),
                "XDG_CONFIG_HOME": str(root / "config"),
                "XDG_CACHE_HOME": str(root / "cache"),
                "XDG_DATA_HOME": str(root / "data"),
                "XDG_RUNTIME_DIR": str(runtime_dir),
                "QT_QPA_PLATFORM": "offscreen",
            }
        )

        # Let this build generate its own complete session schema, then make
        # the tab contain an unsaved edit that would be lost if overwritten.
        run_editor(binary, environment, log_dir / "seed.log", session_path, False, False)
        session = json.loads(session_path.read_text(encoding="utf-8"))
        if len(session["tabs"]) != 1:
            raise RuntimeError("The seed launch did not create one editor tab")
        session["tabs"][0]["editorText"] = MARKER
        session["tabs"][0]["savedText"] = ""
        original = json.dumps(session, ensure_ascii=False).encode("utf-8")
        session_path.write_bytes(original)

        run_editor(binary, environment, log_dir / "no-restore.log", session_path, True, False, original)
        if session_path.read_bytes() != original:
            raise RuntimeError("--no-restore-session changed the saved session")

        run_editor(binary, environment, log_dir / "normal.log", session_path, False, True)
        restored = json.loads(session_path.read_text(encoding="utf-8"))
        if not restored["tabs"] or restored["tabs"][0]["editorText"] != MARKER:
            raise RuntimeError("An ordinary launch did not restore the preserved editor text")

    print("Session recovery preserved the original bytes and restored the unsaved editor text")


if __name__ == "__main__":
    main()
