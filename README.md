<!--

Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.

-->

# amqpAnalyze

## Introduction

amqpAnalyze is an AMQP analyzer for pcapng (wireshark) files.

## Quick start

Dependencies:
* cmake
* gcc-c++
* git
* libpcap-devel

This is a cmake project. Proceed as follows:

```
git clone https://github.com/kpvdr/amqpAnalyze.git
cd amqpAnalyze
mkdir build
cd build
cmake ..
make
```
  
There is no install for this (yet), so run from build location:

```
./src/amqpAnalyze <pcapng-file>
```
For help with options:

```
./src/amqpAnalyze --help
```

Some sample pcapng files are provided in the test_files directory.
