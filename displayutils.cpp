#include <iostream>

#include "labelutils.h"
#include "displayutils.h"

void displayErrorMessage(const std::string& inputFilename, const std::string& errorFilename)
{
    using namespace std;

    system(c_ClearScreenCommand.c_str());

    cerr << "One or more errors occured!" << endl << endl;
    cerr << "Please check the error report in the error file: " << endl << endl;
    cerr << errorFilename << endl << endl;
    cerr << "Please correct the input file and then try again" << endl << endl;
    cerr << "Input file: " << endl << endl << inputFilename << endl << endl;
    cerr << "Thank you for using LabelCalculator!" << endl << endl;
}

void displaySuccessMessage(const std::string& outputFilename, bool displayFurtherInstructions)
{
    using namespace std;

    system(c_ClearScreenCommand.c_str());

    cout << "The program ended succesfully. " << endl << endl;
    cout << "Please view the output file: " << endl << endl;
    cout << outputFilename << endl << endl;

    if (displayFurtherInstructions)
    {
        cout << "Please go on with the next step by filling in the connectioninput.csv file with the needed info" << endl;
    }

    cout << "Thank you for using LabelCalculator!" << endl << endl;
}

void displayAbortMessage()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "Application terminated by user" << std::endl << std::endl;
}

void displayMenu()
{
    using namespace std;

    cout << "Please choose between following options:" << endl << endl;
    cout << "1 + ENTER: read the defined connections from file connectiondefinitions.csv and write the partial input data into file connectioninput.csv" << endl;
    cout << "2 + ENTER: read the input data from file connectioninput.csv and write the labeling information into file labellingtable.csv" << endl << endl;
    cout << "Choose any other combination + ENTER or directly press ENTER to exit the application" << endl << endl;
}

void displayVersion()
{
    system(c_ClearScreenCommand.c_str());
    std::cout << "LabelCalculator v1.0" << std::endl << std::endl;
}
