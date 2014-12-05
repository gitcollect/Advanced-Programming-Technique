// Threaded two-dimensional Discrete FFT transform
// Qichao Chu
// ECE8893 Project 2

#include <iostream>
#include <string>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Complex.h"
#include "InputImage.h"
#define PI 3.141592653589

// You will likely need global variables indicating how
// many threads there are, and a Complex* that points to the
// 2d image being transformed.

using namespace std;

//Global Variables
//Complex* ImageData;
int ImageWidth;
int ImageHeight;
Complex* ImageDatas;
Complex* ImageData1D;
Complex* ImageData2D;
Complex* ImageData2DT;
int n;
InputImage* image;
int nThreads = 64;
int barray[64];
pthread_mutex_t barraymutex;
pthread_mutex_t matrixmutex;

struct arg_struct {
    int arg1;
    Complex* arg2;
};
struct arg_struct args;

// Function to reverse bits in an unsigned integer
// This assumes there is a global variable N that is the
// number of points in the 1D transform.
unsigned ReverseBits(unsigned v,unsigned N)
{ //  Provided to students
  unsigned n = N; // Size of array (which is even 2 power k value)
  unsigned r = 0; // Return value
   
  for (--n; n > 0; n >>= 1)
    {
      r <<= 1;        // Shift return value
      r |= (v & 0x1); // Merge in next bit
      v >>= 1;        // Shift reversal value
    }
  return r;
}

void ReverseAll(Complex* H,int N)
{
    Complex* oh = new Complex[N];
    for (int i = 0; i < N; i++)
    {
      oh[ReverseBits(i,N)] = H[i]; 
    }
    for (int i = 0; i < N; i++)
    {
      H[i] = oh[i]; 
    }
}

int ArraySum()
{
  int j = 0;
  for (int i = 0;i < nThreads; i++ )
  {
      j = j+ barray[i];
  }
  return j;
}
void ArrayReset()
{
   for (int i = 0;i<nThreads;i++)
   {
      barray[i]=0;
   }
}
// GRAD Students implement the following 2 functions.
// Undergrads can use the built-in barriers in pthreads.

// Call MyBarrier_Init once in main
void MyBarrier_Set(int N)// you will likely need some parameters)
{
   barray[N] = 1;
}

// Each thread calls MyBarrier after completing the row-wise DFT
void MyBarrier() // Again likely need parameters
{
  while(ArraySum() != nThreads)
  { }
}


                    
void Transform1D(Complex* H, int N)
{
  // Implement the efficient Danielson-Lanczos DFT here.
  // "h" is an input/output parameter
  // "N" is the size of the array (assume even power of 2)
  ReverseAll(H,N);
  int l = 0,j = 0,k = 0, step=0, m = log2(N);  
  Complex w, delta, temp;

  for(l = 0; l < m; l++)  
  {  
        step = 1 << (l+1);
        w = Complex(1,0);
        delta = Complex(cos(2*PI/step),-sin(2*PI/step));
        for(j = 0; j < step/2; j++)  
        {  
            for(k = 0; k < N/step; k++)  
            {  
                temp = H[k* step + j +step/2]*w;
                H[k*step + step/2 + j] = H[k * step + j] - temp;  
                H[k*step +j]      = H[k * step + j] + temp;  
            }
            w = w * delta; 
        }  
  } 
}

void* Transform2DTHread(void *v)
{ // This is the thread startign point.  "v" is the thread number
  // Calculate 1d DFT for assigned rows
  // wait for all to complete
  // Calculate 1d DFT for assigned columns
  // Decrement active count and signal main if all complete
  //cout << nThreads << endl;
  long myId = (long)v;
  Complex* Line = new Complex[ImageWidth];
  for (int i = (myId*ImageHeight/nThreads) ; i<((myId+1)*ImageHeight/nThreads);i++ )
  {
    for (int j = 0 ; j < ImageWidth ; j++)
    {
      Line[j] = ImageDatas[i*ImageWidth+j];
    }
    Transform1D(Line, ImageWidth);
    
    for (int j = 0 ; j < ImageWidth ; j++)
    {
      pthread_mutex_lock(&matrixmutex);
      ImageDatas[i*ImageWidth+j] = Line[j];
      pthread_mutex_unlock(&matrixmutex);
    }
    
  }
  pthread_mutex_lock(&barraymutex);
  MyBarrier_Set(myId);
  //cout<<ArraySum()<<endl;
  pthread_mutex_unlock(&barraymutex);
 
  return 0;
}

void Transform2D(const char* inputFN) 
{ // Do the 2D transform here.
  cout << nThreads << endl;
  image = new InputImage(inputFN);  // Create the helper object for reading the image
  //ImageData = image.GetImageData();  // Create the global pointer to the image array data
  ImageWidth = image->GetWidth();
  ImageHeight = image->GetHeight();
  ImageData1D = new Complex[ImageWidth*ImageHeight];
  ImageDatas = new Complex[ImageWidth*ImageHeight];
  ImageData2D = new Complex[ImageWidth*ImageHeight];
  ImageData2DT= new Complex[ImageWidth*ImageHeight];
  ImageDatas = image->GetImageData();

  // Create 16 threads
  // Wait for all threads complete
  // Write the transformed data

  for(int i = 0; i < nThreads; ++i)
  {
    // Create the thread
    pthread_t pt; // pThread variable (output param from create)
    // Third param is the thread starting function
    // Fourth param is passed to the thread starting function
    pthread_create(&pt, 0, Transform2DTHread, (void *)i);
  }
  MyBarrier();
  ArrayReset();

  for (int m=0;m<ImageWidth;m++)
  {
    for (int u=0;u<ImageHeight;u++)
    {
       ImageData1D[u+m*ImageHeight] = ImageDatas[u+m*ImageHeight];
    }
  }
  for (int m=0;m<ImageWidth;m++)
  {
    for (int u=0;u<ImageHeight;u++)
    {
       ImageDatas[u+ImageHeight*m] = ImageData1D[u*ImageWidth+m];
    }
  }
  //InputImage image2("my1d.txt");
  image->SaveImageData("my1d.txt", ImageData1D, ImageWidth, ImageHeight);
  cout << "1d is finished" << endl;
  
  // Create the thread
  for(int i = 0; i < nThreads; ++i)
  {
    pthread_t pts; 
    pthread_create(&pts, 0, Transform2DTHread, (void *)i);
  }
  MyBarrier();
  ArrayReset();

  for (int m=0;m<ImageWidth;m++)
  {
    for (int u=0;u<ImageHeight;u++)
    {
       ImageData2D[m+u*ImageWidth]=ImageDatas[u+m*ImageHeight];
    }
  }
  
  //InputImage image3("my2d.txt");
  image->SaveImageData("my2d.txt", ImageData2D, ImageWidth, ImageHeight);
  cout << "2d is finished" << endl;
}


//Inverse Function
void iTransform2D() 
{ // Do the 2D transform here.
  
  for (int m=0;m<ImageWidth;m++)
  {
    for (int u=0;u<ImageHeight;u++)
    {
       ImageDatas[m+u*ImageWidth]=Complex(ImageData2D[u+m*ImageHeight].real,-ImageData2D[u+m*ImageHeight].imag);
    }
  }

  // Create 16 threads
  // Wait for all threads complete
  // Write the transformed data

  for(int i = 0; i < nThreads; ++i)
  {
    // Create the thread
    pthread_t pt; // pThread variable (output param from create)
    // Third param is the thread starting function
    // Fourth param is passed to the thread starting function
    pthread_create(&pt, 0, Transform2DTHread, (void *)i);
  }
  MyBarrier();
  ArrayReset();
  for (int m=0;m<ImageWidth;m++)
  {
    for (int u=0;u<ImageHeight;u++)
    {
       ImageData1D[u+m*ImageHeight] = Complex(ImageDatas[u+m*ImageHeight].real/ImageWidth, ImageDatas[u+m*ImageHeight].imag/ImageWidth);
    }
  }
  for (int m=0;m<ImageWidth;m++)
  {
    for (int u=0;u<ImageHeight;u++)
    {
       ImageDatas[u+ImageHeight*m] = ImageData1D[u*ImageWidth+m];
    }
  }
  
  // Create the thread
  for(int i = 0; i < nThreads; ++i)
  {
    pthread_t pts; 
    pthread_create(&pts, 0, Transform2DTHread, (void *)i);
  }
  MyBarrier();
  ArrayReset();
  for (int m=0;m<ImageWidth;m++)
  {
    for (int u=0;u<ImageHeight;u++)
    {
       ImageData2D[m+u*ImageWidth]= Complex(ImageDatas[m+u*ImageHeight].real/ImageWidth,-ImageDatas[m+u*ImageHeight].imag/ImageWidth);
    }
  }
  image->SaveImageData("myiv.txt", ImageData2D, ImageWidth, ImageHeight);
  cout << "iv is finished" << endl;
}

int main(int argc, char** argv)
{
  string fn("largeTower.txt"); // default file name
  if (argc > 1) fn = string(argv[1]);  // if name specified on cmd line
  if (argc > 2) {
      nThreads =atoi(argv[1]);
      fn = string(argv[2]);
  }
  cout << fn << endl;
  cout << nThreads << endl;
  pthread_mutex_init(&barraymutex,0);
  pthread_mutex_init(&matrixmutex,0);
  ArrayReset();
  Transform2D(fn.c_str()); // Perform the transform.
  iTransform2D();
}  
