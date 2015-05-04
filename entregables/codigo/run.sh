#!/bin/bash
cd keyBoard
make
bochs &
cd ../keySnake
make
bochs &
cd ../snakeDown
make
bochs &
cd ../snakeHash
make
bochs &
cd ../snakeLeft
make
bochs &
cd ../snakeRigth
make
bochs &
cd ../snakeUp
make
bochs &