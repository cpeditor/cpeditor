# Declare characters used by this game.
define s = Character(None, kind=nvl, what_prefix="\"", what_suffix="\"", what_color="#c8ffc8")
define m = Character(_("Me"), color="#c8c8ff")

define menu = nvl_menu
default book = False

# The game starts here.
label start:
    # Start by playing some music.
    play music "illurock.opus"

    scene bg lecturehall
    with fade

    window show

    "bla bla\n"
    "bla {b}bla{/b}"

    window hide
    nvl clear

    scene bg uni
    with fade
    window show

    show sylvie green normal
    with dissolve
    window show

    menu:
        "To ask her later.":
            jump later


label rightaway:
    nvl clear
    show sylvie green smile
    m """bla..."
    "bla bla bla."""

label start:

python:
    player_health = max(player_health - damage, 0)
    f = lambda x: print(x)

label start:
    $ flag = True
    $ romance_points += 1
    $ f = lambda x: print(x)

init 1 python:
    # The bad ending is always unlocked.
    persistent.endings.add("bad_ending")

    f = lambda x: print(x)

label start:
