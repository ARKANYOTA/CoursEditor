# CoursEditor-TUI (AY)

## Version C++


| :warning: ne marche que sur linux
|---

### Dépendances

- gcc (pour compiler) ou autre compilateur c++
- jsonhpp

L'installer sous arch:
```bash
yay -Sy gcc
```

Insaller [Jsonhpp](https://github.com/open-source-parsers/jsoncpp)
A faire dans le dossier tmp ou home:
```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install jsoncpp
```

### Compiler et exécuter
#### Compiler le code

```bash
make compile
ou
g++ main.cpp
```

#### Exécuter le code

```bash
make execute
ou
./a.out
```

#### Faire les 2 en meme temps

```bash
make a
```


### Commandes une fois l'application lancer

```markdown
TODO
```

### Le fichier config "~/.config/cours2022.conf"

TODO

### TODO List:

- La TODO Liste de main.cpp

### D'autres trucs à savoir

| :warning: Aucune action est annulable, donc avant de faire une action, vérifiez votre action
| ---

