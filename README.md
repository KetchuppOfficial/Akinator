![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

# Akinator

This project is a task of the programming course by [Ilya Dedinsky (aka ded32)](https://github.com/ded32).

## General information

**Akinator** is a program that works like a wide known game with the same name. My program differs from the real Akinator in having 2 additional usage modes. The first one gives the definition of a character, if his name is in data base. The second one compares two character from the data base.

## Build and run

**Akinator** is released for Linux only.

**Step 1:** Clone this repository.
```bash
git clone git@github.com:KetchuppOfficial/Akinator.git
cd Akinator
```

**Step 2:** Clone submodule.
```bash
git submodule init
git submodule update
```

**Step 3:** Install Graphviz, if it's not installed.
```bash
sudo apt install graphviz
```

**Step 4:** Install eSpeak, if it's not installed.
```bash
sudo apt install espeak
```

**Step 5:** Build the project.
```bash
make
```

You'll see something like this:

```bash
username@machine:~/Akinator$ make
Collecting dependencies for "src/Stack.c"...
Collecting dependencies for "src/Akinator.c"...
Collecting dependencies for "src/main.c"...
Compiling "src/main.c"...
Compiling "src/Akinator.c"...
Compiling "src/Stack.c"...
Collecting dependencies for "src/My_Lib.c"...
Compiling "src/My_Lib.c"...
ar: creating My_Lib.a
Linking project...
```

**Step 5:** Running.
```bash
make run IN=input_file_name
```
The program won't work, if you don't specify **input_file_name**.

## Examples

After running the program you'll see the start menu like the one in the picture:

![menu](/examples/menu.png)

You should choose one of given modes by writing its number.

### Akinator mode

If you choose "Akinator mode", the program will start asking you question about a character you've thought of. You should answer by typing "y" or "n". Other input cases won't be accepted, and the program will ask you again. You can see the whole game below.

![akinator_mode](/examples/akinator_mode.png)

It can turn out that there is no your character in the Akinator data base. That's why a way to add characters is provided.

![add](/examples/add.png)

If you think of Barney Stinson and ask Akinator to guess who you're thinking of, the program will already have this character in its data base.

![after_adding](/examples/after_adding.png)

### Definition mode

You can use this mode to get the definition of a character from the data base. It is somehow like reading a definition from the explanatory dictionary.

![definition mode](/examples/definition_mode.png)

### Comparison mode

"Comparison mode" helps to compare two characters from the data base.

![comparison_mode](/examples/comparison_mode.png)
