# smartmet-press aka. qdpress

Reads querydata and definiton file as input
Outputs PostScript code or text

See [Wiki documentation](https://github.com/fmidev/smartmet-press/wiki) for help in usage

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
