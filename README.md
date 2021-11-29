# Net-Charge-PH219-Project
A course project for the course PH 219 at IIT Bombay, under Professor Sadhana Dash.

Team: Ayush, Anurag, Mukesh, Paritosh, Pravind, Rehmat, Samyak, Shreyas

_Aim_

To analyse, using ROOT (CERN), p-p collisions at sqrt(s) = 13 TeV generated with the Pythia 8 Monte Carlo event generator, and study the fluctuations in net charge against the multiplicity of the collisions.

The data is uploaded on Google Drive [here](https://drive.google.com/file/d/1-juCBeJ-iHpsX2ynMLhP__Elk4kPsHSm/view?usp=sharing), in the form of a `.root` file.

### Contents

- _Code Files_ contains the ROOT macros which were written to obtain the results.
- _Results_ contains images of the plots obtained.
- _PH219_Project_Report_ is the final project report.


### Running the code
Open the ROOT command line in the `Code Files` directory, place the data file (`13TeV_CR0_RHoff.root`) in the same directory, then run the following code.
```
.L read.cpp
.x plot1and2.cpp
.x Plot_3.cpp
.x Plot_4.cpp
.x Plot_4n.cpp
.x plot_5to9.cpp
```
All the plotting codes require `read.cpp` to be loaded beforehand.

----

The _Project Report_ has been uploaded in this directory. The Overleaf document is [here](https://www.overleaf.com/read/kvtnkkrmwjbf).
