RNet - Realistic random network generator
=========================================

Installation instructions of RNet
Copyright (C) 2013 Matheus Caldas Santos
This file is part of RNet 0.3

RNet 0.3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RNet 0.3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.



RNet is a realistic random network generator. It generates a social network from
some statistical parameters. It deliberately assign communities to each node to 
achieve realistic social behavior.


Compilation
-----------

- Required:

G++ 	- The GNU Compiler Collection (GCC) is a compiler system produced by the
	GNU Project supporting various programming languages. GCC is a key
	component of the GNU toolchain.
CMake 	- Cmake os a cross-platform free software program for managing the build
        process of software using a compiler-independent method.

- Optional:

CCMake	- The "ccmake" executable is the CMake curses interface.


- Commands:

 cd rnet/build
 cmake ../src
 make -j4
 ./rnet
