/**
 * WARNING When changing the copyright dates on new years day, make sure to
 * update the licenses in this file as well!
 *
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "credits.hpp"

#include <iostream>

static const char *license = "Copyright (C) 2020 Tristan\n\
\n\
All Rights Reserved.\n\
\n\
Redistribution and use in source and binary forms, with or without\n\
modification, are permitted provided that the following conditions are met:\n\
\n\
1. Redistributions of source code must retain the above copyright notice,\n\
   this list of conditions and the following disclaimer.\n\
\n\
2. Redistributions in binary form must reproduce the above copyright notice,\n\
   this list of conditions and the following disclaimer in the documentation\n\
   and/or other materials provided with the distribution.\n\
\n\
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n\
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n\
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n\
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE\n\
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n\
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n\
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n\
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n\
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n\
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n\
POSSIBILITY OF SUCH DAMAGE.\n";

static const char *opensslLicense = "This product includes software developed by the OpenSSL Project\n\
for use in the OpenSSL Toolkit (http://www.openssl.org/)\n\
\n\
Copyright (c) 1998-2011 The OpenSSL Project.  All rights reserved.\n\
\n\
Redistribution and use in source and binary forms, with or without\n\
modification, are permitted provided that the following conditions\n\
are met:\n\
\n\
1. Redistributions of source code must retain the above copyright\n\
   notice, this list of conditions and the following disclaimer. \n\
\n\
2. Redistributions in binary form must reproduce the above copyright\n\
   notice, this list of conditions and the following disclaimer in\n\
   the documentation and/or other materials provided with the\n\
   distribution.\n\
\n\
3. All advertising materials mentioning features or use of this\n\
   software must display the following acknowledgment:\n\
   \"This product includes software developed by the OpenSSL Project\n\
   for use in the OpenSSL Toolkit. (http://www.openssl.org/)\"\n\
\n\
4. The names \"OpenSSL Toolkit\" and \"OpenSSL Project\" must not be used to\n\
   endorse or promote products derived from this software without\n\
   prior written permission. For written permission, please contact\n\
   openssl-core@openssl.org.\n\
\n\
5. Products derived from this software may not be called \"OpenSSL\"\n\
   nor may \"OpenSSL\" appear in their names without prior written\n\
   permission of the OpenSSL Project.\n\
\n\
6. Redistributions of any form whatsoever must retain the following\n\
   acknowledgment:\n\
   \"This product includes software developed by the OpenSSL Project\n\
   for use in the OpenSSL Toolkit (http://www.openssl.org/)\"\n\
\n\
THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY\n\
EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n\
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n\
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR\n\
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,\n\
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT\n\
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n\
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n\
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,\n\
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n\
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED\n\
OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
This product includes cryptographic software written by Eric Young\n\
(eay@cryptsoft.com).  This product includes software written by Tim\n\
Hudson (tjh@cryptsoft.com).\n\
\n\
Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)\n\
All rights reserved.\n\
\n\
This package is an SSL implementation written\n\
by Eric Young (eay@cryptsoft.com).\n\
The implementation was written so as to conform with Netscapes SSL.\n\
\n\
This library is free for commercial and non-commercial use as long as\n\
the following conditions are aheared to.  The following conditions\n\
apply to all code found in this distribution, be it the RC4, RSA,\n\
lhash, DES, etc., code; not just the SSL code.  The SSL documentation\n\
included with this distribution is covered by the same copyright terms\n\
except that the holder is Tim Hudson (tjh@cryptsoft.com).\n\
\n\
Copyright remains Eric Young's, and as such any Copyright notices in\n\
the code are not to be removed.\n\
If this package is used in a product, Eric Young should be given attribution\n\
as the author of the parts of the library used.\n\
This can be in the form of a textual message at program startup or\n\
in documentation (online or textual) provided with the package.\n\
\n\
Redistribution and use in source and binary forms, with or without\n\
modification, are permitted provided that the following conditions\n\
are met:\n\
1. Redistributions of source code must retain the copyright\n\
   notice, this list of conditions and the following disclaimer.\n\
2. Redistributions in binary form must reproduce the above copyright\n\
   notice, this list of conditions and the following disclaimer in the\n\
   documentation and/or other materials provided with the distribution.\n\
3. All advertising materials mentioning features or use of this software\n\
   must display the following acknowledgement:\n\
   \"This product includes cryptographic software written by\n\
    Eric Young (eay@cryptsoft.com)\"\n\
   The word 'cryptographic' can be left out if the rouines from the library\n\
   being used are not cryptographic related :-).\n\
4. If you include any Windows specific code (or a derivative thereof) from \n\
   the apps directory (application code) you must include an acknowledgement:\n\
   \"This product includes software written by Tim Hudson (tjh@cryptsoft.com)\"\n\
\n\
THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND\n\
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n\
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n\
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE\n\
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n\
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS\n\
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n\
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT\n\
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY\n\
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF\n\
SUCH DAMAGE.\n\
\n\
The licence and distribution terms for any publically available version or\n\
derivative of this code cannot be changed.  i.e. this code cannot simply be\n\
copied and put under another distribution licence\n\
[including the GNU Public Licence.]\n";

static const char *glewLicense = "The OpenGL Extension Wrangler Library\n\
Copyright (C) 2002-2007, Milan Ikits <milan ikits[]ieee org>\n\
Copyright (C) 2002-2007, Marcelo E. Magallon <mmagallo[]debian org>\n\
Copyright (C) 2002, Lev Povalahev\n\
All rights reserved.\n\
\n\
Redistribution and use in source and binary forms, with or without \n\
modification, are permitted provided that the following conditions are met:\n\
\n\
* Redistributions of source code must retain the above copyright notice, \n\
  this list of conditions and the following disclaimer.\n\
* Redistributions in binary form must reproduce the above copyright notice, \n\
  this list of conditions and the following disclaimer in the documentation \n\
  and/or other materials provided with the distribution.\n\
* The name of the author may be used to endorse or promote products \n\
  derived from this software without specific prior written permission.\n\
\n\
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" \n\
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE \n\
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n\
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE \n\
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR \n\
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF \n\
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n\
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n\
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n\
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF\n\
THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
\n\
Mesa 3-D graphics library\n\
Version:  7.0\n\
\n\
Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.\n\
\n\
Permission is hereby granted, free of charge, to any person obtaining a\n\
copy of this software and associated documentation files (the \"Software\"),\n\
to deal in the Software without restriction, including without limitation\n\
the rights to use, copy, modify, merge, publish, distribute, sublicense,\n\
and/or sell copies of the Software, and to permit persons to whom the\n\
Software is furnished to do so, subject to the following conditions:\n\
\n\
The above copyright notice and this permission notice shall be included\n\
in all copies or substantial portions of the Software.\n\
\n\
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS\n\
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n\
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL\n\
BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN\n\
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN\n\
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
\n\
Copyright (c) 2007 The Khronos Group Inc.\n\
\n\
Permission is hereby granted, free of charge, to any person obtaining a\n\
copy of this software and/or associated documentation files (the\n\
\"Materials\"), to deal in the Materials without restriction, including\n\
without limitation the rights to use, copy, modify, merge, publish,\n\
distribute, sublicense, and/or sell copies of the Materials, and to\n\
permit persons to whom the Materials are furnished to do so, subject to\n\
the following conditions:\n\
\n\
The above copyright notice and this permission notice shall be included\n\
in all copies or substantial portions of the Materials.\n\
\n\
THE MATERIALS ARE PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n\
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n\
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\n\
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY\n\
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,\n\
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE\n\
MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.\n";

static const char *freeTypeLicense = "Portions of this software are copyright © 2020 The FreeType\n\
    Project (www.freetype.org).  All rights reserved.\n\
The FreeType Project LICENSE\n\
                    ----------------------------\n\
\n\
                            2006-Jan-27\n\
\n\
                    Copyright 1996-2002, 2006 by\n\
          David Turner, Robert Wilhelm, and Werner Lemberg\n\
\n\
\n\
\n\
Introduction\n\
============\n\
\n\
  The FreeType  Project is distributed in  several archive packages;\n\
  some of them may contain, in addition to the FreeType font engine,\n\
  various tools and  contributions which rely on, or  relate to, the\n\
  FreeType Project.\n\
\n\
  This  license applies  to all  files found  in such  packages, and\n\
  which do not  fall under their own explicit  license.  The license\n\
  affects  thus  the  FreeType   font  engine,  the  test  programs,\n\
  documentation and makefiles, at the very least.\n\
\n\
  This  license   was  inspired  by  the  BSD,   Artistic,  and  IJG\n\
  (Independent JPEG  Group) licenses, which  all encourage inclusion\n\
  and  use of  free  software in  commercial  and freeware  products\n\
  alike.  As a consequence, its main points are that:\n\
\n\
    o We don't promise that this software works. However, we will be\n\
      interested in any kind of bug reports. (`as is' distribution)\n\
\n\
    o You can  use this software for whatever you  want, in parts or\n\
      full form, without having to pay us. (`royalty-free' usage)\n\
\n\
    o You may not pretend that  you wrote this software.  If you use\n\
      it, or  only parts of it,  in a program,  you must acknowledge\n\
      somewhere  in  your  documentation  that  you  have  used  the\n\
      FreeType code. (`credits')\n\
\n\
  We  specifically  permit  and  encourage  the  inclusion  of  this\n\
  software, with  or without modifications,  in commercial products.\n\
  We  disclaim  all warranties  covering  The  FreeType Project  and\n\
  assume no liability related to The FreeType Project.\n\
\n\
\n\
  Finally,  many  people  asked  us  for  a  preferred  form  for  a\n\
  credit/disclaimer to use in compliance with this license.  We thus\n\
  encourage you to use the following text:\n\
\n\
   \"\"\"\n\
    Portions of this software are copyright © <year> The FreeType\n\
    Project (www.freetype.org).  All rights reserved.\n\
   \"\"\"\n\
\n\
  Please replace <year> with the value from the FreeType version you\n\
  actually use.\n\
\n\
\n\
Legal Terms\n\
===========\n\
\n\
0. Definitions\n\
--------------\n\
\n\
  Throughout this license,  the terms `package', `FreeType Project',\n\
  and  `FreeType  archive' refer  to  the  set  of files  originally\n\
  distributed  by the  authors  (David Turner,  Robert Wilhelm,  and\n\
  Werner Lemberg) as the `FreeType Project', be they named as alpha,\n\
  beta or final release.\n\
\n\
  `You' refers to  the licensee, or person using  the project, where\n\
  `using' is a generic term including compiling the project's source\n\
  code as  well as linking it  to form a  `program' or `executable'.\n\
  This  program is  referred to  as  `a program  using the  FreeType\n\
  engine'.\n\
\n\
  This  license applies  to all  files distributed  in  the original\n\
  FreeType  Project,   including  all  source   code,  binaries  and\n\
  documentation,  unless  otherwise  stated   in  the  file  in  its\n\
  original, unmodified form as  distributed in the original archive.\n\
  If you are  unsure whether or not a particular  file is covered by\n\
  this license, you must contact us to verify this.\n\
\n\
  The FreeType  Project is copyright (C) 1996-2000  by David Turner,\n\
  Robert Wilhelm, and Werner Lemberg.  All rights reserved except as\n\
  specified below.\n\
\n\
1. No Warranty\n\
--------------\n\
\n\
  THE FREETYPE PROJECT  IS PROVIDED `AS IS' WITHOUT  WARRANTY OF ANY\n\
  KIND, EITHER  EXPRESS OR IMPLIED,  INCLUDING, BUT NOT  LIMITED TO,\n\
  WARRANTIES  OF  MERCHANTABILITY   AND  FITNESS  FOR  A  PARTICULAR\n\
  PURPOSE.  IN NO EVENT WILL ANY OF THE AUTHORS OR COPYRIGHT HOLDERS\n\
  BE LIABLE  FOR ANY DAMAGES CAUSED  BY THE USE OR  THE INABILITY TO\n\
  USE, OF THE FREETYPE PROJECT.\n\
\n\
2. Redistribution\n\
-----------------\n\
\n\
  This  license  grants  a  worldwide, royalty-free,  perpetual  and\n\
  irrevocable right  and license to use,  execute, perform, compile,\n\
  display,  copy,   create  derivative  works   of,  distribute  and\n\
  sublicense the  FreeType Project (in  both source and  object code\n\
  forms)  and  derivative works  thereof  for  any  purpose; and  to\n\
  authorize others  to exercise  some or all  of the  rights granted\n\
  herein, subject to the following conditions:\n\
\n\
    o Redistribution of  source code  must retain this  license file\n\
      (`FTL.TXT') unaltered; any  additions, deletions or changes to\n\
      the original  files must be clearly  indicated in accompanying\n\
      documentation.   The  copyright   notices  of  the  unaltered,\n\
      original  files must  be  preserved in  all  copies of  source\n\
      files.\n\
\n\
    o Redistribution in binary form must provide a  disclaimer  that\n\
      states  that  the software is based in part of the work of the\n\
      FreeType Team,  in  the  distribution  documentation.  We also\n\
      encourage you to put an URL to the FreeType web page  in  your\n\
      documentation, though this isn't mandatory.\n\
\n\
  These conditions  apply to any  software derived from or  based on\n\
  the FreeType Project,  not just the unmodified files.   If you use\n\
  our work, you  must acknowledge us.  However, no  fee need be paid\n\
  to us.\n\
\n\
3. Advertising\n\
--------------\n\
\n\
  Neither the  FreeType authors and  contributors nor you  shall use\n\
  the name of the  other for commercial, advertising, or promotional\n\
  purposes without specific prior written permission.\n\
\n\
  We suggest,  but do not require, that  you use one or  more of the\n\
  following phrases to refer  to this software in your documentation\n\
  or advertising  materials: `FreeType Project',  `FreeType Engine',\n\
  `FreeType library', or `FreeType Distribution'.\n\
\n\
  As  you have  not signed  this license,  you are  not  required to\n\
  accept  it.   However,  as  the FreeType  Project  is  copyrighted\n\
  material, only  this license, or  another one contracted  with the\n\
  authors, grants you  the right to use, distribute,  and modify it.\n\
  Therefore,  by  using,  distributing,  or modifying  the  FreeType\n\
  Project, you indicate that you understand and accept all the terms\n\
  of this license.\n\
\n\
4. Contacts\n\
-----------\n\
\n\
  There are two mailing lists related to FreeType:\n\
\n\
    o freetype@nongnu.org\n\
\n\
      Discusses general use and applications of FreeType, as well as\n\
      future and  wanted additions to the  library and distribution.\n\
      If  you are looking  for support,  start in  this list  if you\n\
      haven't found anything to help you in the documentation.\n\
\n\
    o freetype-devel@nongnu.org\n\
\n\
      Discusses bugs,  as well  as engine internals,  design issues,\n\
      specific licenses, porting, etc.\n\
\n\
  Our home page can be found at\n\
\n\
    https://www.freetype.org\n";

static const char *glfwLicense = "Portions of GLFW are included in this software.\n\
Copyright (c) 2002-2006 Marcus Geelnard\n\
\n\
Copyright (c) 2006-2019 Camilla Löwy\n\
\n\
This software is provided 'as-is', without any express or implied\n\
warranty. In no event will the authors be held liable for any damages\n\
arising from the use of this software.\n\
\n\
Permission is granted to anyone to use this software for any purpose,\n\
including commercial applications, and to alter it and redistribute it\n\
freely, subject to the following restrictions:\n\
\n\
1. The origin of this software must not be misrepresented; you must not\n\
   claim that you wrote the original software. If you use this software\n\
   in a product, an acknowledgment in the product documentation would\n\
   be appreciated but is not required.\n\
\n\
2. Altered source versions must be plainly marked as such, and must not\n\
   be misrepresented as being the original software.\n\
\n\
3. This notice may not be removed or altered from any source\n\
   distribution.\n";

void
Credits::PrintToCommandLine() {
	std::cout << ">>>> WebEngine Credits <<<<\n"
			  << license << std::endl
			  << opensslLicense << std::endl
			  << glewLicense << std::endl
			  << freeTypeLicense << std::endl
			  << glfwLicense << std::endl
			  << std::endl;
}
