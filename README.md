Starter project with OCaml + Reason

Create a local opam switch
```sh
make create-switch
```

Install the development dependencies
```sh
make dev-deps
```

To build the project:
```sh
dune build
```

And to run it:
```sh
dune exec ./bin/main.exe
```

Nice to have packages for development:
- ocamlformat
- ocaml-lsp-server
- utop
- reason (To enable formatting reason files with refmt)
