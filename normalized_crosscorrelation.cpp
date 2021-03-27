#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <numeric>
#include <math.h>
#include <algorithm>

using namespace std;

// This function returns a boolean whether the input 
// is a valid input without extra characters
bool isDouble(string input, double& number)
{
  stringstream ss(input);

  ss >> number;

  // If the stringstream is indeed a double with no extra characters
  // And is at the end of the line
  if (!ss.fail() && ss.eof())
  {
    return true;
  }
  else
  {
    return false;
  }
}

//readFile is responsible for obtaining the starting 
//index the input file which is stored in startingIndex
//readFile also creates a vector containing all the inputs
double readFile(string filename, vector<double>& inputData, 
  bool& error)
{
  ifstream f;
  string line;
  string extraTest;
  double valid;
  double startingIndex = 0;
  error = false;
  
  f.open(filename);
  // If file not found, prompt the user to re-type 
  //the filename
  if (!f)  
  {
    cerr << "File not found. Please input a valid filename.\n";
    error = true;
    return 0;
  }
  
  // Read first line
  getline(f, line);
  stringstream ss;
  
  // Assign the first line to ss
  ss << line;
  
  // Get first entity (separated by spaces of first line)
  // and assign it to startIndex
  // Code below does the checking too if first entity is invalid
  ss >> extraTest;
  if (isDouble(extraTest, valid))
  {
    startingIndex = valid;
  }

  else // Prompts the user to check the file and re-input.
  { 
    cout << "\nInvalid input. Please check your input file." << endl;
    cout << "Here is the invalid line: " << line << endl; 
    // Here is the invalid line outputs empty space if file is empty.
    cout << "Check your file and input the filename again." << endl;
    error = true;
    return 0;
  }

  // If there is still another element 
  // ie input 1 input 2 (e.g. -3 5)
  if ((ss >> valid)) 
  {
    inputData.push_back(valid);
  }
  // If none, then the first value is already 
  // the first datapoint and startingIndex = 0
  else 
  {
    inputData.push_back(startingIndex);
    startingIndex = 0;
  }
  
  // Read succeeding lines until EOF is reached
  while (getline(f, line)) 
  {
    stringstream ss;
    ss << line;
    if (ss >> valid)
    {
      inputData.push_back(valid);
    }
  }
  
  f.close();
  return startingIndex;
}

// showData prints out all the elements in a vector
// range-based for loop
// in case the user wants to see the data read into each vector
void showData(vector<double> data)
{
  for (double dataPoint : data)
  {
    cout << dataPoint << endl;
  }
}

// average is responsible for taking the averages of the elements 
// in a given vector. This is used to refine the input signal vector
double average(vector<double> data)
{
  double ave;
  
  ave = accumulate(data.begin(), data.end(), 0.0)/data.size();
  
  return ave;
}

// subtractAverages subtracts the average 
// from the ith vector element.
vector<double> subtractAverages(vector<double> inputData)
{
  vector<double> newData;
  double ave;
  
  ave = average(inputData);
  for (int i = 0; i < inputData.size(); i++)
  {
    newData.push_back(inputData[i] - ave);
  }

  return newData;
}

// autocorrelation takes the crosscorrelation 
// of the input signal vector to itself while letting l = 0
double autocorrelation(vector<double> inputData)
{
  double sum = 0;
  
  for (int i = 0; i < inputData.size(); i++)
  {
    sum += inputData[i] * inputData[i];
  }

  return sum;
}

// contains returns true if a value is contained in the vector
// for instance, if we have vector y and say y[-100] as the element,
// contains will return false because there is the value at y[-100]
// which will be a random number and is not in y. 
// We use this to determine whether a certain matching of 
// x[n] * y[n-shift] is valid. 
bool contains(vector<double> vec, double &elem)
{
  bool result = false;
  
  if (find(vec.begin(), vec.end(), elem) != vec.end())
  {
    result = true;
  }

  return result;
}

// crossCorrelation responsible for creating the array 
// of possible crosscorrelation values given two signals
vector <double> crossCorrelation(vector<double> x, vector<double> y,
  int startX, int startY, int shift, int &outputStartIndex)
{
  vector <double> Z;
  int endX = startX + x.size() - 1; //end index of x
  int endY = startY + y.size() - 1; //end index of y
  
  // start index of cross correlated signal
  int ccStartIndex = startX - endY; 
  // end index of cross correlated signal
  int ccEndIndex = endX - startY; 
  int duration = ccEndIndex - ccStartIndex + 1;
  double z;

  for (shift = ccStartIndex; shift <= ccEndIndex; shift++) 
  {
    z = 0;
    for (int i = 0; i < x.size(); i++)
    {
      if (startX < startY || startX == startY)
      {
        if ((contains(y, y[i - (startY - startX) - shift]) == 1))
        {
          z += x[i] * y[i - (startY - startX) - shift];
        }
        else
        {
          continue;
        }
      }
      else if (startX > startY)
      {
        if ((contains(y, y[i + (startX - startY) - shift]) == 1))
        {
          z += x[i] * y[i + (startX - startY) - shift];
        }
        else
        {
          continue;
        }
      }
    }
    Z.push_back(z);
  }
  outputStartIndex = ccStartIndex;
  
  return Z;
}

// normalizationCoefficients returns the normalization 
// coefficient needed for normalizing the resulting crosscorrelation;
// reducing them into the values that range from -1 to 1
double normalizationCoefficient(vector<double> x, vector<double> y)
{
  return sqrt(autocorrelation(x)*autocorrelation(y));
}

// normalizedCrosscorrelation outputs the resulting vector 
// containing all the possible normalized crosscorrelation 
// values for a given pair of signal vectors
vector <double> normalizedCrosscorrelation(double normCoef, 
  vector <double> crosscorreVec)
{
  vector <double> normVec;
  
  for (double dataPoint : crosscorreVec)
  {
    normVec.push_back(dataPoint / normCoef);
  }
  return normVec;
}

// outputFile writes the values from the normalizedCrosscorrelation
// vector to a given filename in .txt format
void outputFile(string filename, int startIndex, 
  vector<double> outputData)
{
  ofstream f;
  cout << endl;
  cout << "Program is saving your output to: " << filename;
  
  f.open(filename);
  
  if (startIndex != 0) // To copy input format of (index) (data_1)
  {
    f << startIndex << ' ';
  }

  for (double output : outputData)
  {
    f << output << endl;
  }

  f.close();
}

// main function
int main()
{
  vector <double> xRaw;
  vector <double> yRaw;
  bool error = false;
  int startX, startY;
  int shift;
  int outputStartIndex;
  double z;

  do
  {
    string xFilename;
    cout << "\nPlease input filename for x: ";
    cin >> xFilename;
    startX = readFile(xFilename, xRaw, error);    
  } 
  while (error == true);
  
  do
  {
    string yFilename;
    cout << "\nPlease input filename for y: ";
    cin >> yFilename;
    startY = readFile(yFilename, yRaw, error);
  }
  while (error == true);

  string outputFilename;
  cout << "\nPlease input your desired output filename for the ";
  cout << "\nnormalized cross-correlated signals: ";
  cin >> outputFilename;

  vector <double> xSubtractedAverages = subtractAverages(xRaw); 
  vector <double> ySubtractedAverages = subtractAverages(yRaw);

  cout << endl << "Input raw X data: " << endl;
  showData(xRaw);

  cout << endl << "Input X data with subtracted averages: " << endl;
  showData(xSubtractedAverages); 

  cout << endl << "Input raw Y data: " << endl;
  showData(yRaw);

  cout << endl << "Input Y data with subtracted averages: " << endl;
  showData(ySubtractedAverages);

  double autocorrelationX = autocorrelation(xSubtractedAverages);
  double autocorrelationY = autocorrelation(ySubtractedAverages);
  double normCoef = normalizationCoefficient(xSubtractedAverages, 
    ySubtractedAverages);
  
  cout << "\nAutocorrelation of X is: " << autocorrelationX << endl;
  cout << "Autocorrelation of Y is: " << autocorrelationY << endl; 
  cout << "Normalization coefficient is: " << normCoef << endl; 

  vector <double> result = crossCorrelation(xSubtractedAverages, 
    ySubtractedAverages, startX, startY, shift, outputStartIndex);
  vector <double> normVec = normalizedCrosscorrelation(normCoef, 
    result);
  
  cout << "\nNormalized cross correlated signals starting at index ";
  cout << outputStartIndex << endl;
  showData(normVec);

  outputFile(outputFilename, outputStartIndex, normVec);
}