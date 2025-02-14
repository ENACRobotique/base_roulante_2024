# Code base roulante 2024

- Récupérer les submodules:

    `git submodule update --init --recursive`

## dépendances

## Divers

 ```
 sudo usermod -aG dialout robot 
 sudo apt install dfu-util
 sudo apt install make
 ```

### Protobuf

```
sudo apt install protobuf-compiler python3-protobuf
```

### ARM GNU toolchain

 - Télécharger la toolchain depuis https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads.
 - Extraire l'archive dans `/opt` et ajouter au PATH le sous-répertoire `bin`.
 
### PERL pour le générateur board.cfg -> board.h

```
sudo apt install libmodern-perl-perl libxml-libxml-perl libfile-which-perl libsyntax-keyword-match-perl
sudo perl -MCPAN -e 'install String::LCSS'
```


### EmbeddedProto

Changer la version de protobuf requise par EmbeddedProto: dans `EmbeddedProto/generator/pyproject.toml` remplacer `"protobuf>=4.25.1,<5"` par `"protobuf>=4.21.1,<5"`.


```
sudo apt install python-is-python3 python3-pip python3-venv
cd EmbeddedProto/
python3 setup.py -c
```


It _might_ be necessary to execute this command if protobuf generation fail complaining about not finding  embedded_proto_options_pb2:

`
protoc -I generator --python_out=generator/EmbeddedProto embedded_proto_options.proto
`





