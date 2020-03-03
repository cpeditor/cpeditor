## Versioning rules for CP Editor

These rules apply to CP Editor since v6.0. The versions before v6.0 may not satisfy these rules.

### Pull requests

Pull requests which propose a feature should be based on the master branch.

Pull requests which fix a bug should be based on the oldest one of the three active branches (stable > beta > master) which contains the bug.

### Version number format

Versioning of CP Editor is based on [Semantic Versioning](https://semver.org/), but no `-rc1`, `-beta`, `-stable` is included, so every version number must be strictly in the format `$X.$Y.$Z`, where `$X`, `$Y`, and `$Z` are all non-negative integers without leading zeros.

The version numbers must be continuous, i.e. no directly from `6.1.2` to `6.3.1` without `6.2.1` is allowed.

### Release cycle

At the beginning of a release cycle, we do the following things:

1. Create a new branch `v$X.$Y` from the master branch.
2. Make a new pre-release `$X.$Y.1`. (`$X.$Y.0` is for the master branch.)
3. Make a stable release from the last version (usually `$X.$(Y-1).$Z`, or maybe `$(X-1).$W.$Z`).

A release cycle can last for 1~4 weeks, depending on the working progress and the importance of the new features.

### Working on branches

We work on three branches at the same time: the master branch, the latest pre-release branch (called the beta branch), and the latest stable release branch (called the stable branch). Other branches are no longer maintained, they may contain bugs that are known by the maintainers.

On the master branch, we make all changes including new features and bug fixes.

On the version branches, we fix bugs only, no new features will be added. Releases are made on version branches only.

When fixing a bug, it should be fixed on the oldest one of the three active branches (stable > beta > master) which contains the bug, and then get merged in to the later branches. Version branches shouldn't be ahead of the master branch. (This rule begins at v6.1.)

Code-unrelated changes (e.g. issue template changes, CI changes) apply to the master branch only if it's not necessary to apply to other branches.

### When to make new releases

We make new releases in these cases:

1. At the beginning of a release cycle, we regularly make two releases, which is described in the [Release cycle part](#Release-cycle).
2. We make stable releases whenever a bug is fixed on the stable branch.
3. We make pre-releases when a critical bug is fixed on the beta branch.

We don't change a pre-release into a stable release. If no bugs are found, we can make a stable release which is exactly the same as the pre-release.

### Changelogs

Changelogs are manually written for new releases, which contain notable changes in the latest release.

The [CHANGELOG.md](CHANGELOG.md) contains different logs on different branches:

- On the master branch:
  1. The UNRELEASED changes on the master branch.
  2. The changes between each two adjacent MINOR versions (usually between `$X.$Y.1` and `$X.$(Y+1).1`, or maybe between `$X$.$Y.1` and `$(X+1).0.1`). No patch version is included.
- On version branches:
  1. The changelogs of the master branch before the `$X.$Y.1` was released.
  2. The UNRELEASED changes on the version branch.
  3. The changes between each two adjacent PATCH versions.

We update [CHANGELOG.md](CHANGELOG.md) in these cases:

1. When any notable changes are made, update the UNRELEASED changes on the branch where the changes are made.
2. When releasing a new MINOR version, move the UNRELEASED changes on the master branch to "changes between MINOR versions".
3. When releasing a new PATCH version, move the UNRELEASED changes on the corresponding version branch to "changes between PATCH versions".

A changelog between versions should be like this:

```Markdown
<!-- ## v$X.$Y for changes between MINOR versions on the master branch -->

<!-- ## $X.$Y.$Z for stable releases on the version branch -->

<!-- ## $X.$Y.$Z (Beta) for beta releases on the version branch -->

<!-- "Nothing changed." here when releasing a stable release which is exactly the same as the latest pre-release at the beginning of a release cycle -->

### Added

- Foo is added.
- Bar is added.

### Fixed

- Something is fixed.

### Changed

- Another thing is changed.
```

### The complete workflow of making a new release

1. Additional steps for creating a new MINOR version at the beginning of a release cycle:
   1. Create a new branch `v$X.$Y` based on the master branch.
   2. After creating the new branch, create a commit on the master branch which contains exactly the following:
      - Change the version in [CMakeLists.txt](../CMakeLists.txt) to `$X.$(Y+1).0`.
      - Move the UNRELEASED changes to the changelog of `v$X.$Y` in [CHANGELOG.md](CHANGELOG.md).
      - The commit subject is `Start a new release cycle for v$X.$(Y+1)`.
2. Create a commit on the version branch which contains exactly the following:
   - Change the version in [CMakeLists.txt](../CMakeLists.txt) to `$X.$Y.$Z`.
   - Move the UNRELEASED changes to the changelog of `$X.$Y.$Z` in [CHANGELOG.md](CHANGELOG.md).
   - The commit subject is `Release $X.$Y.$Z`.
3. Tag the commit created at the step 2 as `$X.$Y.$Z`.
4. Push both the commit and the tag.
5. Edit the changelog of the GitHub Release, make it non-draft, and mark it as a pre-release if it is.
6. Update badges in [README.md](../README.md) on all the three branches.
