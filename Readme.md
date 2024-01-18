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

- Télécharger protobuf depuis les releases github : https://github.com/protocolbuffers/protobuf/releases/tag/v25.2
- Extraire l'archive au chemin voulu, par exemple : `/opt/protoc-25.2`.
- Ajouter le chemin à votre `~/.bashrc` : `export PATH="/opt/protoc-25.2/bin:$PATH"`


### ARM GNU toolchain

 - Télécharger la toolchain depuis https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads.
 - Extraire l'archive dans `/opt` et ajouter au PATH le sous-répertoire `bin`.
  ```
  sudo add-apt-repository ppa:deadsnakes/ppa
  sudo apt install python3.8
  sudo apt install libncursesw5
  ```
 
### PERL pour le générateur board.cfg -> board.h

```
sudo apt install libc6:i386 libncurses5:i386 libstdc++6:i386
sudo apt install libmodern-perl-perl libxml-libxml-perl
sudo perl -MCPAN -e 'install String::LCSS'
```


### EmbeddedProto

```
sudo apt install python-is-python3 python3-pip python3-venv
cd EmbeddedProto/
python3 setup.py -c
```

It _might_ be necessary to execute this command if protobuf generation fail complaining about not finding  embedded_proto_options_pb2:

`
protoc -I generator --python_out=generator/EmbeddedProto embedded_proto_options.proto
`





