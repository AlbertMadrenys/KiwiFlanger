KiwiFlanger
===========================================

KiwiFlanger is a console application written in C++ that allows to modify a sound file
with the classic flanger DSP effect. It is written in C++, and uses
[libsndfile](https://github.com/libsndfile/libsndfile) for the file input and
output, and [KiwiWaves](https://github.com/AlbertMadrenys/KiwiWaves) for the DSP core.

In conjunction with KiwiWaves, created by the same author, KiwiFlanger is
part of the final project for the _Musical_Signal_Processing_ module in _Maynooth_University_.

Build
----------------------------------------------

The program is built with CMake. It needs both lbsndfile and KiwiWaves
as external dependencies. Make sure to have previously installed these.

``` 
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

If building the program is not possible due to not finding the libraries,
add path prefixes in the `.cmake` files present inside the folder of the same name.
To do that, add more prefixes after the keyword `PATH_SUFFIXES` of the
`find_path()` and `find_library()` instructions.

Using
----------------------------------------------

A console environment is used to comunicate with the core program.
First, an input and output name files have to be entered.
If the input file has more than one channel, one of the channels will have to be
selected. The output will always be mono.

The flanger has a loop time average of 1.25 ms, being 0.5 ms the minimum and 2 ms the maximum.
The amount of depth and feedback of the effect can be set, although feedback cannot surpass 1.
Because the loop time will be modulated with an cosine-wave LFO, the frequency of it can be set as well.
Lastly, as the feedback will be filtered using a first-order low-pass filter, the cutoff frequency
can be changed depending on the desired brigthness.
 
License
----------------------------------------------

	Copyright (C) 2024 Albert Madrenys Planas

	This software is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this software; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
