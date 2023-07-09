/* ------------------------------------------------------------------------------------------------------ */
//																										  //
//									This a program for MRI image manipulation							  //
//																										  //
/* ------------------------------------------------------------------------------------------------------ */


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <windows.h>
#include <iostream>
#include <fstream>


using namespace cv;
using namespace std;

/* ----------------------------------- Class for the MRI Images --------------------------- */


class MRIimage
{
public:
	/*------------------------------------- Defining variables --------------------------------------------*/
	string LocationOfDirectory;
	string ImageName;
	float BrightnessValue = 0;
	float ContrastValue = 50;
	int MedianFilterFrequency = 0;

	/* -------------------------------- Sunctions for changing the parameters -------------------------------- */
	void BrightnessIncrease() {
		BrightnessValue += 10;
	}

	void BrightnessDecrease() {
		BrightnessValue -= 10;
	}

	void ContrastIncrease() {
		ContrastValue += 10;
	}

	void ContrastDecrease() {
		ContrastValue -= 10;
	}
	// ------------------ Default constructor ----------------
	MRIimage(string Location, string Name) {
		LocationOfDirectory = Location;
		ImageName = Name;
		
	}

	/* ------------------------------------------------------------------------------------------------------ */

	 
	/* ------------------The Display function to diaplay the image---------------------- */
	void Display() {
		Mat image = cv::imread(LocationOfDirectory + "/" + ImageName);
		image.convertTo(image, -1, ContrastValue / 50, BrightnessValue);			// printing out all the details of the image
		cout << "\nContrast value: " << ContrastValue;
		cout << " Brightness Value: " << BrightnessValue << " " << " Median Filter Frequency: " << MedianFilterFrequency << "\n";
		imshow("MRI Image", image);													// show image
	}

	/* ----------------Function to adjust the brightness and contrast-------------------- */
	void BrightnessAndContrastAdjustment() {
		Mat image = cv::imread(LocationOfDirectory + "/" + ImageName);
		image.convertTo(image, -1, ContrastValue / 50, BrightnessValue);
		cout << "Contrast value: " << ContrastValue;								// print brightness value
		cout << " Brightness Value: " << BrightnessValue <<" ";						// print contrast value
		imshow("MRI Image", image);													// show image
	}

	/* Function to apply the median filter */
	void ApplyMedianFilter(string WhatToDo = "m") {
		Mat image = cv::imread(LocationOfDirectory + "/" + ImageName);
		image.convertTo(image, -1, ContrastValue / 50, BrightnessValue);

		if (WhatToDo == "m")
		{
			/*<------------------------------------------ MEDIAN FILTER --------------------------------------------->*/
			// Apply Median Filter
			int kernelSize = 3; // 3x3 neighborhood size, adjust as per your requirement
			int padding = kernelSize / 2;

			// Traverse the image excluding the boundary pixels
			for (int row = padding; row < image.rows - padding; row++) {
				for (int col = padding; col < image.cols - padding; col++) {
					std::vector<uchar> neighborhood;

					// Collect pixel values in the neighborhood
					for (int i = -padding; i <= padding; i++) {
						for (int j = -padding; j <= padding; j++) {
							neighborhood.push_back(image.at<uchar>(row + i, col + j));
						}
					}

					// Sort the pixel values
					std::sort(neighborhood.begin(), neighborhood.end());

					// Get the median value from the sorted neighborhood
					uchar medianValue = neighborhood[neighborhood.size() / 2];

					// Update the current pixel with the median value
					image.at<uchar>(row, col) = medianValue;
				}
			}
			/* ------------------------------------------------------------------------------------------------------ */

			// Adjust brightness and contrast of the image
			image.convertTo(image, -1, ContrastValue / 50.0, BrightnessValue);

			// Increment the median filter frequency count
			MedianFilterFrequency++;

			// Display the adjusted image
			cv::imshow("MRI Image", image);


		}
		else {  // show the unchanged image
			cv::imshow("MRI Image", image);
			MedianFilterFrequency--;
			if (MedianFilterFrequency <= 0) {
				MedianFilterFrequency = 0;
			}

		}
	}

	/* ------------------ Function that saves the new file with a new name ------------------ */
	void Save() {
		Mat image = cv::imread(LocationOfDirectory + "/" + ImageName);
		image.convertTo(image, -1, ContrastValue / 50.0, BrightnessValue);

		/* Applying the median filter according to the frequency of the filter */

		if (MedianFilterFrequency > 0)
		{
			/*<-------------------------------------------- MEDIAN FILTER ------------------------------------------->*/

			for (int i = 0; i < MedianFilterFrequency; i++) {
				int kernelSize = 3; // 3x3 neighborhood size, adjust as per your requirement
				int padding = kernelSize / 2;
				for (int row = padding; row < image.rows - padding; row++) {
					for (int col = padding; col < image.cols - padding; col++) {
						std::vector<uchar> neighborhood;
						for (int i = -padding; i <= padding; i++) {
							for (int j = -padding; j <= padding; j++) {
								neighborhood.push_back(image.at<uchar>(row + i, col + j));
							}
						}
						std::sort(neighborhood.begin(), neighborhood.end());
						uchar medianValue = neighborhood[neighborhood.size() / 2];
						image.at<uchar>(row, col) = medianValue;
					}
				}
				image.convertTo(image, -1, ContrastValue / 50.0, BrightnessValue);
			}

			/* ------------------------------------------------------------------------------------------------------ */
		}


		/* ----------------------------- Extracting the file name and extension --------------------------------- */
		
		string nameWithoutExtension, fileExtension;

		size_t separatorPos = ImageName.find_last_of('/');
		if (separatorPos != std::string::npos) {
																						// Extract the file name without the path
			std::string fileName = ImageName.substr(separatorPos + 1);

																						// Find the last occurrence of the file extension separator character '.'
			size_t extensionPos = ImageName.find_last_of('.');
			if (extensionPos != std::string::npos) {
																						// Extract the file name without the extension
				nameWithoutExtension = ImageName.substr(0, extensionPos);

																						// Extract the file extension
				fileExtension = ImageName.substr(extensionPos + 1);
				
																						// Print the extracted values
				std::cout << "\nFile Name: " << nameWithoutExtension << std::endl;
				std::cout << "\nFile Extension: " << fileExtension << std::endl;
			}
		}

		/* ------------------------------------------------------------------------------------------------------ */


		//---------------------making the changed output path------------------------------------

		string outputPath = LocationOfDirectory + "/" + nameWithoutExtension + "_B" + to_string(BrightnessValue) + "_C" + to_string(ContrastValue) + "_MDF" + to_string(MedianFilterFrequency) + "." + fileExtension;
		cout << "Output Path: " << outputPath<<endl;									// printing the output path
		cv::imwrite(outputPath, image);													// writing the changed image to the new file at the output file
	}

};

		/* ------------------------------------------------------------------------------------------------------------ */


int main()
{

	/* ----------------Defining the variables to be used in the project--------------- */
	string MetaFilePath; string FileName;
	int NumberOfFiles;
	float BrightnessIncrement = 0, ContrastIncrement = 0;


	/* while loop so that the program can be run infinite times till exit key press */
	while (true) {
		vector<string>* FileNames = new vector<string>;			// Dynamically allocating the file names variable in a vector to store all the file names

	/* ------------------------------------------------------------------------------------------------------ */


		//<-------------------------------------- MENU ---------------------------------------------------------->


		cout << setw(10) << "\n\t\tThis is a program for MRI image manipulation\n";

		cout << "Please enter the path to the meta file (use forward slashed '/') give the ful path: ";		// Prompting the user for input
		ifstream file;											// Making an input file stream object

		while (true)											// Keep asking the user until the right file path is given
		{
			getline(cin, MetaFilePath);							// storing the meta file path in a variable

			file.open(MetaFilePath);							// opening the file with the file path

			/* printing out an error if the file is not opened and returning error number " 1 " */
			if (!file) {
				std::cerr << "Failed to open metafile." << std::endl;
				cout << "Please enter the right file path: ";
				continue;										// going to the top if wrong file path given and asking again
			}
			break;
		}

		//<------------------------------------------------------------------------------------------------------->



		/*------------------- Extracting the number of files and their names --------------------------------------*/

		file.ignore(16);										// ignoring first 16 inputs that correspond to "Number of files "
		file >> NumberOfFiles;									// adding the number of files to a variable

		cout << "Number of files included in the meta file: " << NumberOfFiles << "\n"; // printing out the number of files included in the meta file

		while (getline(file, FileName))							// iterating over the rest of the file and adding the file name and paths to a vector
		{
			if (!FileName.empty())
			{
				FileNames->push_back(FileName);					// using push back (as it is a vector the siz increases dynamically)
			}

		}

		file.close();											// closing the meta file as it is not needed

		/* ---------------printing out the file names stored in the vector----------------------------------------- */

		cout << "\n\nThe file names stored in vector are as follows\n";

		for (int i = 0; i < NumberOfFiles; i++)
		{
			cout << "\n" << (*FileNames)[i];
		}


		/* Extracting Directory's path from the file path given */
		// assuming the image folder is in the same Directory as the meta file

		size_t lastSeparator = MetaFilePath.find_last_of("/\\");			// finding the position of the last seperator "/"


		/*---------------Extracting the file path--------------------------------------------------------------------------*/
		std::string DirectoryPath = MetaFilePath.substr(0, lastSeparator);	// using substr to get a sub string till the point of the position of the last seperator "/"

		std::cout << "File Path: " << MetaFilePath << std::endl;			// Printing the file path
		std::cout << "Directory Path: " << DirectoryPath << std::endl;		// Printing the Directory path

		vector<MRIimage>* MRIimages = new vector<MRIimage>;					// Making a vector with custom data type MRIimage made with new operator so it exits on heap and is modifiable on runtime

		for (int i = 0; i < NumberOfFiles; i++)
		{
			MRIimage MI(DirectoryPath, (*FileNames)[i]);					// making a dummy object "MI" with the directory pah and file name
			MRIimages->push_back(MI);										// Pushing the dummy object on the the MRIimages vector
		}

		for (int i = 0; i < NumberOfFiles; i++) {
			cout << "\n" << (*MRIimages)[i].ImageName;						// Printing the names of the MRI images from the mri images vector
		}

		
		string option;

		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);				// Making a handle to control the colour

		//<------------------------------------------ MENU ----------------------------------------------->

		SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);			// changing the colour to blue

		cout << "\n\n\t--------What do you want to do-------\n";
		cout << "\t1. Continue (open the files).\n";
		cout << "\t2. Go back to the top and enter a new meta file path to open.\n";
		cout << "\t3. Exit the program completely.\n";
		cout << "Enter the option: ";

		SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);		// change the colour back to default

		//<----------------------------------------------------------------------------------------------->

		while(true)
		{
			getline(cin, option);
			if (option.empty() || (option != "1" && option != "2" && option != "3"))
			{
				cout << "Please enter the right option: ";					// Prompting the user to enter the right right option
				continue;													// if not ask again
			}
			break;															// break if file name right
		}



		if (option == "1") {  // continuing


			//<------------------------------------------- MENU ---------------------------------------------->

			SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);	// change the colour to blue

			cout << "\n\t--------Functionality--------\n";
			cout << "\tw/W: Move up\n";
			cout << "\tq/Q: Move down\n";
			cout << "\tr/R: Increase Brightness\n";
			cout << "\te/E: Decrease Brightness\n";
			cout << "\tt/T: Increase Contrast\n";
			cout << "\ty/Y: Decrease Contrast\n";
			cout << "\tm/M: Apply the median filter (press continuously to increase the frequency)\n";
			cout << "\tn/N: Compltely remove the median filter effect\n";
			cout << "\tEscape key closes the open cv window and you go back to the top.\n\n\n";

			SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // change back to default

			//<---------------------------------------- Opening the first file ----------------------------------------->

			string imagePath = DirectoryPath + "/" + (*FileNames)[0]; // making image path

			cv::Mat image = cv::imread(imagePath);					// Read the image at the first given path

			cv::namedWindow("MRI Image", cv::WINDOW_NORMAL);

			cv::imshow("MRI Image", image);							// show the image

			//<---------------------------------------------------------------------------------------------------------->


			int CurrentIndex = 0;									// Defining the Current index to keep track of the open file

			/* ----------------------------------------- Configuring the buttons presses ------------------------------------- */

			while (true) {
				// Wait for a key press event
				int keyCode = cv::waitKey(0);


				if (keyCode == 'q' || keyCode == 'Q') {				// Action when "q" or "Q" is pressed
					// Move one image down
					CurrentIndex--;
					if (CurrentIndex <= 0) {
						CurrentIndex = 0;
					}
					(*MRIimages)[CurrentIndex].Display();

				}
				else if (keyCode == 'w' || keyCode == 'W') {		// Action when "w" or "W" is pressed
					// Move one image up
					CurrentIndex++;
					if (CurrentIndex >= (NumberOfFiles - 1)) {
						CurrentIndex = (NumberOfFiles - 1);
					}
					(*MRIimages)[CurrentIndex].Display();

				}

				else if (keyCode == 'r' || keyCode == 'R')			// Action when "r" or "R" is pressed
				{
					//increase brightness
					(*MRIimages)[CurrentIndex].BrightnessIncrease();
					(*MRIimages)[CurrentIndex].BrightnessAndContrastAdjustment();
					cout << (*MRIimages)[CurrentIndex].ImageName << "'s " << "brightness increased\n";

				}

				else if (keyCode == 'e' || keyCode == 'E')			// Action when "e" or "E" is pressed
				{
					//decrease brightness
					(*MRIimages)[CurrentIndex].BrightnessDecrease();
					(*MRIimages)[CurrentIndex].BrightnessAndContrastAdjustment();
					cout << (*MRIimages)[CurrentIndex].ImageName << "'s " << "brightness decreased\n";

				}

				else if (keyCode == 't' || keyCode == 'T')			// Action when "t" or "T" is pressed
				{
					//increase contrast
					(*MRIimages)[CurrentIndex].ContrastIncrease();
					(*MRIimages)[CurrentIndex].BrightnessAndContrastAdjustment();
					cout << (*MRIimages)[CurrentIndex].ImageName << "'s " << "contrast increased\n";

				}

				else if (keyCode == 'y' || keyCode == 'Y')			// Action when "y" or "Y" is pressed
				{
					//decrease contrast
					(*MRIimages)[CurrentIndex].ContrastDecrease();
					(*MRIimages)[CurrentIndex].BrightnessAndContrastAdjustment();
					cout << (*MRIimages)[CurrentIndex].ImageName << "'s " << "contrast decreased\n";

				}

				else if (keyCode == 'm' || keyCode == 'M') {		// Action when "m" or "M" is pressed
					// Applying the median filter
					(*MRIimages)[CurrentIndex].ApplyMedianFilter();
				}

				else if (keyCode == 'n' || keyCode == 'n') {		// Action when "n" or "N" is pressed
					// Removing all the median filter effect
					(*MRIimages)[CurrentIndex].ApplyMedianFilter("n");
				}

				else if (keyCode == 's' || keyCode == 'S') {		// Action when "s" or "S" is pressed
					(*MRIimages)[CurrentIndex].Save();
				}

				else if (keyCode == 27){ // 27 is the ASCII code for the 'Esc'
					// Exit the loop hence the application when the 'Esc' key is pressed
					cv::destroyWindow("MRI Image");					// destroy the window
					break;
				}
				if (cv::getWindowProperty("MRI Image", cv::WND_PROP_VISIBLE) < 1) { // Check if the window is closed or does not exist
					break;
				}
			}
		}
		/* -------------------------------------------------------------------------------------------------------------------------------- */
		else if (option == "2") {
			cout << "\n\tYou have been taken back to the top\n";
			continue;
		}

		else if (option == "3") {
			cout << "\n\n\t\tBye!!!\n";
			break;
		}
	}

	return 0;				// returns 0 if everything goes fine
}