# pre-commit

## Installation

Before you can run hooks, you need to have the pre-commit package manager installed.

Using pip:

```shell
pip install pre-commit
```

In a python project, add the following to your requirements.txt (or requirements-dev.txt):

```shell
pre-commit
```

As a 0-dependency [zipapp](https://docs.python.org/3/library/zipapp.html):

- locate and download the `.pyz` file from the [github releases](https://github.com/pre-commit/pre-commit/releases)
- run `python pre-commit-#.#.#.pyz ...` in place of `pre-commit ...`

Using [homebrew](https://brew.sh/):

```shell
brew install pre-commit
```

Using [conda](https://conda.io/) (via [conda-forge](https://conda-forge.org/)):

```shell
conda install -c conda-forge pre-commit
```

## Quick start

### 1. Install pre-commit

- follow the [install](#installation) instructions above
- `pre-commit --version` should show you what version you're using

### 2. Install the git hook scripts

- run `pre-commit install` to set up the git hook scripts
- now `pre-commit` will run automatically on `git commit`!

### 3. (optional) Run against all the files ¶

- it's usually a good idea to run the hooks against all of the files when adding new hooks (usually `pre-commit` will only run on the changed files during git hooks)
