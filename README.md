<!-- 
   Targoman: A robust Statistical Machine Translation framework                
                                                                               
   Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                             
                                                                               
   This file is part of Targoman.                                              
                                                                               
   Targoman is free software: you can redistribute it and/or modify            
   it under the terms of the GNU Lesser General Public License as published    
   by the Free Software Foundation, either version 3 of the License, or        
   (at your option) any later version.                                         
                                                                               
   Targoman is distributed in the hope that it will be useful,                 
   but WITHOUT ANY WARRANTY; without even the implied warranty of              
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               
   GNU Lesser General Public License for more details.                         
   You should have received a copy of the GNU Lesser General Public License    
   along with Targoman. If not, see <http://www.gnu.org/licenses/>.            
                                                                               
                                                                             -->
Targoman: A robust Statistical Machine Translation framework     
===================
   Targoman is a robust Statistical Machine Translation framework which can use [moses](statmt.org/moses/) or [jane](www-i6.informatik.rwth-aachen.de/jane/) trained models to provide a robust decoding process. Further information can be found at [Targoman blog](http://blog.targoman.com/en/?cat=3).

See INSTALL for build instructions.

**Targoman Project Tree**
```
  ├── Apps 
  │   ├── E4SMT    
  │   ├── TargomanLoadBalancer    
  │   ├── TargomanSMTConsole     
  │   └── TargomanSMTServer       
  ├── ExternalToolsAndLibs        
  │   └── KenLM                   
  ├── NLPLibs
  │   ├── TargomanLM              
  │   ├── TargomanStem (Not yet published)
  │   └── TargomanTextProcessor
  ├── Scripts 
  ├── Samples
  ├── TargomanCommon 
  ├── TargomanSMT  
  ├── Templates                 
  └── Tests   
  ```
#####**Sub-directories Specification:**

> - **E4SMT**: Application based on TargomanTextProcessor which converts text to IXML and vice versa it also validates XML files and some more stuff.
> - **TargomanLoadBalancer**:Application that monitors translation servers and load-balances requests between them.
> - **TargomanSMTConsole**: Application based on TargomanSMT as a multithreaded console application. 
> - **TargomanSMTServer**: Application based on TargomanSMT as a multithreaded server application. This app comunicates with TargomanLoadBalancer and  can be set-up on clouds.
> - **KenLM**: KenLM Langugae model library 
> - **TargomanLM**: Targoman Language Model Library 
> - **TargomanStem**: Targoman Stemmer Library  (Not yet published) 
> - **TargomanTextProcessor**: Targoman TextProcessor Library. Used to  normalize characters, tokenize text, tag special tokens and vice versa
> - **Scripts**: Train, Tune, and other usefull scripts (Not yet published)
> - **Samples**: Sample config file and corpora to test Targoman. 
> - **TargomanCommon**: Usefull and common tools as a compact library
> - **TargomanSMT**: Core Targoman SMT Library 
> - **Templates**: Templates used to generate subprojects 
> - **Tests**: BlackBox Tests 
