# ITMO University Distributed computing labs (4/5)
Tasks can be found here: https://goo.gl/3XnGJw (RUS). All labs work correct and were passed through ifmo.distributedclass.bot@gmail.com

### Makefile instructions:
* `make` - compile & link
* `make clean` - remove object, log & executable files

## PA1
Program creates communication system using pipes. Child processes notify about START & DONE events via sending messages.

### Run:
`./pa1 -p X`, where <b>X</b> - count of child processes.

## PA2
Using PA1 we can immitate banking system by adding useful work to child processes.

### Run:
`./pa2 -p X y1 ... yX`, where <b>X</b> - count of child processes, <b>yN</b> - process start balance.

## PA3
Same as PA2. Instead of Physical time here is used Lamport time.

## PA4
Working with critical area as child process useful work.

### Run:
`./pa1 -p X [--mutexl]`, where <b>X</b> - count of child processes, <b>--mutexl</b> - tells program to use Lamport mutex algorithm in critical area
