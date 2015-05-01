Example code 
============

To clone this repository you should write this on your terminal

```sh
git clone http://giteit.udp.cl/rfuenzalida/example-code.git
```

To compile code you just type the following on the main directory of this project:

```sh
make
```

To execute the code:

```sh
bochs
```

For this project you need install the following package (just follow the instruction):

```sh
sudo apt-get install build-essential # (just if you don't have it yet.)
sudo apt-get install bochs
sudo apt-get install bochs-x
sudo apt-get install bochs-sdl
```

Take spetial care with the .c pragram.

In case you don't have git installed please do the next:

```sh
sudo apt-get install git
```
Then following the above instructions.
Please, for any bug or issue, write a topic about it on [Q&A Forum](http://cursos.fic.udp.cl/mod/forum/view.php?id=106)

________________________________

### Possible problems and solutions:

- problem executiong bochs with `display_library: sdl` option, chenge it to `display_library: x`