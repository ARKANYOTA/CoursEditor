# CoursEditor-TUI (AY)


| :warning: Pour le moment le programme ne marche que sur linux.
|---

### Dépendances

- gcc (pour compiler) ou autre compilateur c++
- jsonhpp

L'installer sous arch:
```bash
yay -Sy gcc jsoncpp
```

Insaller [Jsoncpp](https://github.com/open-source-parsers/jsoncpp)
A faire dans le dossier tmp ou home:
```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install jsoncpp
```
### Compilation et execution et installation
#### Compiler

```bash
make compile
```

#### Compiler et Installer 

```bash
make compile install  # ou make
```

#### Executer pour les tests (Demande de l'installer pour avoir les configs)
```bash
make execute
```

### Config

Le fichier config est dans `/home/ay/.config/cours2022.json`
Le fichier log    est dans `/home/ay/.config/cours2022.log`

### TODO List:

- La TODO Liste de main.cpp

### D'autres trucs à savoir

| :warning: Aucune action est annulable, donc avant de faire une action, vérifiez votre action
| ---

### Commandes une fois l'application lancer, Help

```markdown
TODO
```
