0. INTRODUCTION

This application is used for creating a labelling table for cable connections between racked devices: servers, storage, switches, power distribution units etc. Both power and data connections are supported. One label is created for each connected port. The labels can be printed and attached to cables.

1. DISCLAIMER

This application hasn't been thoroughly tested and might still contain bugs. Please don't use it in any mission critical environment without performing sufficient testing beforehand. I will take no responsibility for any damages it might cause whatsoever!

2. INSTALLATION AND SETUP

The installation process is simple and contains only a few steps:
- download the codebase from the Git repository: https://github.com/LiviuCP/LabelCalculator.git
- open the project with Qt Creator and build it into any folder to which you have permission

Notes:
- the application is written in standard C++ code so no specific Qt deployment steps are required. Actually other IDEs might be used for compiling it provided that some project setup changes are performed.
- a compiler supporting C++14 is required.
- regarding Qt version I used 5.9.2 or higher but lower versions might be able to do the job as well

3. SUPPORTED OPERATING SYSTEMS

The application has been compiled and tested on Linux (Fedora 28) and Mac OS (Catalina). It might also be compiled on Windows however I haven't done this so far (I only have Linux and OS X machines) so some compilation errors might be expected. Let me know if this happens.

4. RUNNING THE APPLICATION

Once the application has been compiled, when running it the user will be prompted to choose an option:
- option 1: define the connections within rack
- option 2: create the input for each individual point-to-point connection
- option 3: create an empty connection definitions file

Before choosing one of the first two options the user should make sure a valid input file is available:
- for option 1 (defining connections): connectiondefinitions.csv
- for option 2 (entering connection input): connectioninput.csv

The file should be named exactly as above and placed into the Documents folder from the home directory. Depending on the operating system this would be:
- on Mac: /Users/[username]/Documents
- on Linux: /home/[username]/Documents
- on Windows: C:\Users\[username]\Documents

Once the appropriate file is created, please choose the required option and press ENTER. For exiting the application without performing any action simply press the return key.

After choosing option 1 or 2 the application will run the necessary calculations. If the input is valid and no errors occur, an output file will be generated into the same folder.

Depending on chosen option the output file is:
- connectioninput.csv for option 1
- labellingtable.csv for option 2

If the input file contains invalid data, an error file will be generated and the user will be requested to correct the errors. The error file is also written to Documents and is named error.txt.

After correcting the errors the user should repeat the previous steps and check the resulting output file. For more details see next sections.

The user can also choose option 3 for generating an empty connectiondefinitions.csv file. No input file is needed in this step. Then the user can fill in the required definition information and re-run the application with option 1.

To be noted: the application only runs a single option and then exits. The user should re-run the application for executing another option no matter which this is.

5. DATA FILES

Following user data files are being processed by application:
- connection definitions file: connectiondefinitions.csv
- connection input file: connectioninput.csv
- labelling table file: labellingtable.csv

In addition to these, an error file might be created for either of the options 1 and 2. The name of the file is errors.txt.

For the first two options, depending on user choice one of the first two files should be provided. It is required that the exact naming is used and the file is placed into the Documents folder (see section 4), otherwise an error will be triggered and the application will terminate.

Both the labellingtable.csv and error.txt files can only be created by application. First one is generated when choosing option 2 while the other can be created when either of the first two options is executed unsuccessfully.

The output (connectioninput.csv or labellingtable.csv depending on option) and the error file cannot contain data in the same time after running the application:
- if the application finishes successfully the connectioninput.csv (option 1)/ labellingtable.csv (option 2) contains the requested output, namely the connection input template for option 1 or the labelling table for option 2. The error.txt file is empty. A success confirmation is displayed in the CLI.
- if the application finishes with parsing errors, these are contained in error.txt. Any content of the output file file is erased. An error confirmation is displayed in the CLI.

Additionally, when errors which are not parser specific occur they are displayed in detail in the terminal. For example if the input file cannot be opened for reading, this is signalled by application by displaying the error within CLI. The application terminates without anything being written to error.txt file.

For more details see the next sections.

5.1. CONNECTION DEFINITIONS

This file is used when the user chooses option 1 in the application menu. The provided input is used for creating a template connection input file that will be subsequently modified by user for creating the actual input file to be used when executing option 2. It is not mandatory to run this option. The user could create the connection input file from scratch and then use it in option 2. However this step simplifies the creation of the connection input file and helps avoiding potential parsing errors. It also assists in creating the labelling table in an orderly manner.

The connection definitions file consists of a:
- header row
- data rows which build a rack layout

The header row should be present but is not used by application. It might be left empty as well without any impact on the outcome. It preceeds the data rows which start from row 2.

The number of data rows is 50, one for each U position within rack. This corresponds to the maximum rack size (50U). Any additional rows are ignored by application.

The data rows represent the rack layout starting at U50 and ending with the first U position. The data is structured as follows:
- the first column contains the U numbering of the rack. Its only purpose is to guide the user in defining the racked devices and connections but can be left empty as well.
- on the second column the device type can be defined for the corresponding U position
- starting with third column connections to upper placed devices can be defined

To efficiently define device connections the user should perform following steps:
- enter all devices, one per row. This is done by filling in the type of each device on the second column on the lowest U position occupied by the device within rack
- starting from the bottom of the rack, for each device the connected devices and the number of connection points should be defined. This is done by using the columns next to the device type. Each cell should contain the destination (target) device (to which the current device connects) and the number of connection points (cables).

For example, the server located at U5 might connect to two LAN switches, one located at U41 and the other at U42. The number of connection points to each switch is 2. Consequently following data should be entered:
- on row corresponding to U5
  - column 2 (device type): svr
  - column 3 (first connection): 41/2
  - column 4 (second connection): 42/2
- on row corresponding to U41, column 2 (device type): lan
- on row corresponding to U42, column 2 (device type): lan
- no connections to be entered on the switch rows, these have been defined on the server row

Notes:
- for each device, only the devices located above it should be mentioned as connected devices
- the number of connection points should be higher than 0
- there should be no empty cells between device type and defined connections or between any two defined connections
- if the rack has a smaller size, the user should not fill in anything for the rows corresponding to the higher U positions which are not contained within rack. For example if the rack is 36U high, then the rows corresponding to positions 37U-50U should not be filled in.

5.2. CONNECTION INPUT

This file is processed when the user chooses option 2 in the application menu. The provided input is used for creating the final output, namely the labelling table.

The connection input file consists of a:
- header row
- data rows which contain connections between two device ports (point-to-point)

The header row should be present but is not used by the application. It might be left empty as well without impact on the outcome. The data rows (payload) start at row 2.

Each data row contains a point-to-point cable connection between two power or data ports. The row data is structured as follows:
- cable part number
- type of the first device
- U number of the first device
- port parameters for first device
- U number of the second device
- type of the second device
- port parameters for second device

The required port parameters depend on device type. For example a storage device demands following parameters in the exact order: controller number, port type and port number. Each one should be entered in a separate CSV cell following the cell containing the device U number.

It is mandatory to fill in all required port parameters for a specific device type. They should be entered in the right order and contiguously. A guideline for filling in connectioninput.csv can be found in the support file deviceportinput.csv (see source code).

When generating the connectioninput.csv by using option 1, placeholders are being inserted for each port parameter and for the cable PN. These should be replaced by the actual parameter values (e.g. 2 instead of CONTROLLER NUMBER for storage device data ports). This is actually the recommended method of filling in the input file before choosing option 2.

Regarding cable part number, if the same PN is used on several contiguous rows it is sufficient to fill it in for the first of these rows. All following empty rows will have the same part number considered as input.

Last but not least, please be aware that in this step no check is performed that two different devices are connected on each row so take extra caution if creating the connection input file from scratch. As mentioned before it is highly recommended to create it as template with option 1 and then fill in the placeholders with the useful values.

An empty connection definitions file can be created by choosing the third option from the menu.

5.3. LABELLING TABLE

The labelling table is the final output of the application and can be obtained by providing a valid connection input file (connectioninput.csv), running the application and selecting the second option. It is written to labellingtable.csv.
The first table row is the header. All others are data rows.

Each table data row contains a point-to-point cable connection between two devices, namely:
- the cable part number
- description of the first (source) device port
- cable label of the first device port
- description of the second (destination) device port
- cable label of the second device port

There are two types of connections:
- power connections, e.g. server power supply to PDU
- data connections, e.g. storage FC port to SAN switch

The connection type is not necessarily determined by device type, i.e. the PDU can also have a data connection between its management port and a LAN switch. On the other hand each device has a power port that connects to a PDU / extension bar / UPS.

For better clarity and manageability it is recommended to keep power and data connections in separate tables, i.e. run the whole end-to-end labelling table creation process separately for each connection type.

5.4. ERROR FILE

This file contains any parsing errors that prevent the output file from being created. These errors occured when one of the first two options has been selected.

For each row of the input file one or more parsing errors might occur if the input is invalid. When this happens, each encountered error is logged into the error file.

The logging entry contains:
- error code
- error description
- CSV row and/or column where this error occured (or if not applicable the name and path of the input file)
- any corrective actions from user side

The CSV row and/or column can be identified by opening the file in a spreadsheet processing tool. The column number mentioned in the error file corresponds to the column letter displayed by spreadsheet tool, e.g. column number 8 mentioned in the error is column H as displayed by spreadsheet tool. Both the rows and the columns numbering starts at 1.

6. PARSING ERRORS

There are two types of errors:
- "hard" parsing errors
- "soft" parsing errors

Hard parsing errors lead to application termination without any usable output written to file. Actually an empty output file is created or the existing one is flushed. The errors are written to error.txt. The hard parsing errors that might be encountered by user and the corresponding corrective actions are mentioned in the guidelines contained in the support file parsingerrors.csv.

On the other hand the soft parsing errors only have a limited impact affecting only the point-to-point connection where they occurred. They are triggered only for option 2 and only if no hard parsing errors had occurred. These errors are written to the labelling table output file on the appropriate rows. The other rows that are error-free are written correctly into the same file. The error.txt file is left empty and instead each soft error is filled-in into labellingtable.csv within the description and label cells of the device port for which it occurred. It might also be filled-in into the cable PN field if it relates to the (missing) cable part number. Examples of soft errors are the ones that occur when the load segment number is invalid, meaning it hasn't been entered as positive integer by user. The program still displays a successful completion in the command line interface when these errors occur. Consequently the user should check the labelling table and correct the data if needed or manually overwrite the affected entries if a custom labelling is required for them.

Note: a soft error that could occur for each device port type is the one regarding exceeding the maximum number of characters that are allowed in the label (16). If the label calculated based on device port parameter values entered by user exceeds this threshold, two error texts will replace the description and label of each affected port in the labelling table. The description text mentions the actual error and corrective actions while the label text mentions the number of characters above the limit. The user should update the device port parameters in the connection input file so this threshold is no longer exceeded and then execute option 2 again.

7. OTHER ERRORS

Other possible errors are:
- unable to open the input file for reading in either option 1 or 2: file might be missing or the user is lacking required permissions
- unable to open the output file for writing in any of the available options, same possible reasons
- unable to open the error file for writing in any of the first two options, same possible reasons
- setup error, user not recognized etc.

When these errors occur, please make sure:
- your user account is correctly setup
- the required files are existing and reside in the correct directory (Documents).
- you have the right read/write/execute permissions for them

8. MISCELLANEOUS

Currently the application does not support inter-rack connectivity. Also it doesn't check duplicate connections or port-to-port connections validity (e.g. invalid connection between a FC port and a PDU power port). All these might be added in a future version.

For a better understanding of the application options and files, please check the Documentation/Examples folder (see source code), which contains both data and power connections. All required user input or output files except error.txt are provided there. Please note that for both data/power connections the connectioninput_template.csv file has been manually renamed to reflect the fact that it is generated when executing option 1 and that it is only a template that needs to be filled-in prior to executing the second option. However when running the application it is (and should always be) named connectioninput.csv.
