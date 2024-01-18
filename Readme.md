# Code base roulante 2024

- Récupérer les submodules:

    `git submodule update --init --recursive`

## dépendances


### Protobuf

- Télécharger protobuf depuis les releases github : https://github.com/protocolbuffers/protobuf/releases/tag/v25.2
- Extraire l'archive au chemin voulu, par exemple : `/opt/protoc-25.2`.
- Ajouter le chemin à votre `~/.bashrc` : `export PATH="/opt/protoc-25.2/bin:$PATH"`


### ARM GNU toolchain

 - Télécharger la toolchain depuis https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads.
 - Extraire l'archive dans `/opt` et ajouter au PATH le sous-répertoire `bin`.
 
### PERL pour le générateur board.cfg -> board.h

```
sudo apt install libc6:i386 libncurses5:i386 libstdc++6:i386
sudo apt install libmodern-perl-perl libxml-libxml-perl
sudo perl -MCPAN -e 'install String::LCSS'
```


## EmbeddedProto

```
sudo apt install python-is-python3 python3-pip python3-venv
cd EmbeddedProto/
python3 setup.py -c
protoc -I generator --python_out=generator/EmbeddedProto embedded_proto_options.proto
```


