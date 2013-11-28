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


                                        ,::,.                                                      
                                    .:lkOOddkx;.                                                   
                                   ,xdOOOOOOddko:                                                  
                                  .oddOOOOOdddkox,                                                 
                                  ,okdddddddkkoxl;.                                                
                              .:xONdookkkkkoooxl;,                                                 
                          .:xO@W@@NKllxxxxxxllc:,.                                                 
                      .:xON@@NXKOkc,.,:;;;;;:::,.                                                  
    .,cxokool;,.  .;o0N@@NX0Ooc,.      ..,,;;;;.                                                   
   :okddddddddkokKN@NXK0dx;,.              ;ccc,                        .,:;cc;:,.                 
  :ookkddddddkkkOXKOkl:,.                  ,oOOo.                      ,cxxxxxxll;.                
  cxookkkkkkkkkoxx;,.                       lOOd;                     .lxxxxxxxllc:.               
  ,cxxooookkoooxlckOol:.                    ,kOdo.                    ,clxxxkdklc;:.               
  .,;cllxo0XXOxc:;O0KXXXKdx;,.               ;ddkc                    .:cclldOOkc,.                
    .,,::xX@@Nk,...,;lodO0KKKKdoc,.          .oddo,                     .,::lO00d:                 
      ...,KN@@K,       ..,;lodO00KKOol:,.     ,kdkc,..                     ..l0KKO:                
          kN@@Nl             .,,;lodOO00Okx;,,cokkoxxl:.                     .kKXKO;,:,,..         
          ;X@@N0.                 ..,:cxkdOO0Ooooooooxlc.                     :OOdddddkkkol:.      
          .0N@@X;                       ..,;loxxxood00OOdkooxllc;:,,,...    .ckddddddddkkkoox;.    
           oX@@Nk                           ,cllllxxxxokddOO00KKKXXXXXXXK00kokdddddddkkkkooxxlc.   
           :KN@@X,                           ,:;ccc;:,,,..,,:;clxokdOO00KKKkkkkkkkkkkkkoooxxllc:.  
           .ON@@Nk                            ..,,,,..           ....,,:;;loookkkkkoooooxxxlcc;,.  
            lX@@NK,                                                   ,;oOXNdooooooxxxxxllcc;:,,.  
            ,KN@@Xx                                             .,coOX@NNNXK0olllllllllcc;;:,,,.   
            .dX@@N0.                                       .,coON@NNXXK0Odoxc::;;;;;;;;::,,,,,.    
             cXN@NXc                                  .:lkKN@NNXXK0Okoxc:,..  ..,,,,,,,,,,,,..     
             ,0N@@Nd.                            .:xdKN@NNXXK0Okol;:,..          .........         
              kXN@NK:                       ,;xOX@@NNXKK0dkxl;,,..                                 
              ;KN@NXo,..               ,;oOX@NNNXKK0dkxl;,,..                                      
              ,dOdddddkkol:.     .,;oOXNNNNXK00dkxl;,,..                                           
            .cokkddddddkkkkoc:coOXNNNXXK0Odoxc;,,..                                                
           ,xokkkkkkkkkkkkkoONNXXKK0Okoxc:,..             _____  _   _ ______ _______ 
          ,xoookkkkkkkkkooook0Odkolc:,..                 |  __ \| \ | |  ____|__   __| 
          ;xxooooooooooooxxxllc:,..                      | |__) |  \| | |__     | |
         .;llxxxxxxxxxxxxllcc;,.                         |  _  /| . ` |  __|    | |
          ,;ccllllllllllcc;;:,.                          | | \ \| |\  | |____   | |
          .,::;;ccccc;;;::,,,.                           |_|  \_\_| \_|______|  |_| 
            .,,,,,,,,,,,,,,..                                                                      
              ..,,,,,,,,..                                                                         
                  ....                                                                             


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

# cd rnet/build
# cmake ../src
# make -j4
# ./rnet
