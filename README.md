# RmlUi_tests
Aim : gathering simple codes and docs to test RmlUi lib on MorphOS

At time of writing, the most up to date version of the RmlUi lib is 6.0 though, with improvements & changes.
See RmlUi official repo hee : <https://github.com/mikke89/RmlUi#start-of-content>
There are some samples code in that official repo also.


The main purpose of this RmlUi test repo is to test RmlUi code on MorphOS, specifically.
The proposed samples on this site should compile with RmlUi 4.3 
( see original author samples : <https://github.com/mikke89/RmlUi/tree/4.3/Samples> )


Specific "issues" to be processed/tested in MorphOs context is (not limited to):

- specific Fonts loading mechanisms (On Morphos, fonts are added in the system in a dedicated folder)
  Style css file in RmlUi may specify some fonts provided in some project folder but not installed in the system.
  
- Provided path to RmlUi documents in MorphOS (possibly linked to other files, css, rml, ...) 
  seems to be better managed if provided as full path (instead of relative ones - to be confirmed)
  
- Recently updated TingyGL library on MorphOS improves the drawing calls in RmlUi code, 
  but stil to be tested on existing code (BitRiot, ....)
  
Already, this project alows you to check you installed a working version of RmlUi on your MorphOS system,
and it may be usefull ;-)



To start compiling & use the project :

Your MorphOS system should have last tinyGL and SDL2 libraries installed, and last gcc SDK

The RmlUi library should be  installed ( in /usr/local/lib for instance ) 
like the 4.3 version recommended for BitRiot project.


git clone this repo somewhere, then in shell go in this cloned project :

mkdir build

cd build

cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=ppc-morphos-g++-11 -DCMAKE_C_COMPILER=ppc-morphos-gcc-11

(a bin/rmluiTester executable s created after a succesful compilation)

cd /
 
Test rendreding a simple provided RML doc : 

build/bin/rmluiTester <full_path_to_your_cloned__rmlui_tests_project>/assets/my_document_3.rml

The view should be rendereded.


Note: there is a tests_scripts folder with some populated scripts to open some RmlUi docs from assets folder, 
but you'll need to adjust the proposed full path to match your current folder path before using them.
