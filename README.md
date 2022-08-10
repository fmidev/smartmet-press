# smartmet-press aka. qdpress

`qdpress` is the name of the binary.

`smartmet-press` is the name of the RPM-package and this repository

The program reads querydata and a definiton file as input, and makes PostScript or ASCII text files as output.

How a product definition file for qdpress should look like see:
[Syntax for qdpress definition](docs/Syntax-for-qdpress-definition.md)

What each part of the definition means see: [Qdpress Reference Manual](docs/Qdpress-Reference-Manual.md)

## Development (FMI internal)

* Clone repo at dev.dev
* Create a new branch: `git checkout -b feature/<a-new-thing>`
* Make qdpress great again
* Build a local binary file: `make`
* Test that things still work with local `qdpress`-binary
* Update the .spec-file [smartmet-press.spec](smartmet-press.spec)
* Merge branch back to master: `git checkout master; git merge --no-ff feature/<a-new-thing>`
* Create Linux package files: `make rpm`
* Install to dev.dev
* ...
* Wait until monday
* ...
* Copy packages to appropriate `/download/...` subdirectory
* Update repository information `smartrepoupdate`
* Install new version to production machines

Something like that.
