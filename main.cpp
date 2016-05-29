#include <iostream>
#include <fstream>
using namespace std;

class QuadTreeNode
{
	
	friend class QuadTree;
	friend class Image;
	
	public:
	int **imgAry;
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int color;  // could only be 0, 1 or 2
	int rowOffset;
	int colOffset;
	QuadTreeNode* NW_kid;
	QuadTreeNode* SW_kid;
	QuadTreeNode* SE_kid;
	QuadTreeNode* NE_kid;
	
	
	QuadTreeNode()
	{
		NW_kid = NULL;
		SW_kid = NULL;
		SE_kid = NULL;
		NE_kid = NULL;
	}
	
	void printNode(QuadTreeNode* node, ofstream &myOut)
	{
	
		if(node == NULL)
		{
			myOut <<  " NULL";
		}
		else
		{
			myOut << node->color << " " << node->numRows << " " << node->numCols << " " << node->maxVal
			<< " " << node->minVal << " " << node->rowOffset << " " << node->colOffset ;
			if(node->NW_kid == NULL)
					myOut << " NULL";
			else
					myOut << " " <<  node->NW_kid->color;
			if(node->SW_kid == NULL)
					myOut << " NULL";
			else
					myOut << " " << node->SW_kid->color;
			if(node->SE_kid == NULL)
					myOut << " NULL";
			else
					myOut << " " << node->SE_kid->color;
			if(node->NE_kid == NULL)
					myOut << " NULL";
			else
					myOut << " " << node->NE_kid->color ;
			myOut << endl;
		}
		
	}	
	
	~QuadTreeNode()
	{
		delete NW_kid;
		delete SW_kid;
		delete SE_kid;
		delete NE_kid;
	}
};


class Image
{
	friend class QuadTreeNode;
	friend class QuadTree;
	public: 
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	char* inFile;
	char* outFile1;
	char* outFile2;
	
	int** imgAry;
	Image(char* input, char* output2)
	{
		inFile = input;
		outFile2 = output2;
		loadImage();
		testPrint();
	}
	
	void loadImage()
	{
		ifstream myFile(inFile);
		myFile >> numRows;
		myFile >> numCols;
		myFile >> minVal;
		myFile >> maxVal;

		
		makeImage();
			for(int i = 0; i < numRows && !myFile.eof(); i++)
			{
				for(int j = 0; j < numCols && !myFile.eof(); j++)
				{
					myFile >> imgAry[i][j];
				}
			}
			myFile.close();
		return;
	}
	

	void makeImage()
	{
		imgAry = new int*[numRows];
		for(int i = 0; i < numRows; i++)
		{
			imgAry[i] = new int[numCols];
		}
		return; 
	}
	
	void testPrint()
	{
		ofstream myOutFile2(outFile2);
		for(int i = 0; i < numRows; i++)
		{
			for(int j = 0; j < numCols; j++)
			{
				myOutFile2 << imgAry[i][j] <<  " " ;
			}
			myOutFile2 << endl;			
		}
		myOutFile2.close();
		return;
	}
};

class QuadTree
{
	friend class Image;
	friend class QuadTreeNode;
	

	public: 
	QuadTreeNode* rootNode;
	int **imgAry;

	QuadTree(QuadTreeNode* root, int** array)
	{
			rootNode = root;
			rootNode->color = 2;
			rootNode->rowOffset = 0;
			rootNode->colOffset = 0;
			imgAry = array;
	}
	
	
	void computeOffsets(QuadTreeNode * T)
	{
			if(T==NULL || T->numCols <= 1 || T->numRows <= 1)
			{
				return;	
			}
			T->NW_kid = new QuadTreeNode();
			T->NW_kid->rowOffset = T->rowOffset;
			T->NW_kid->colOffset = T->colOffset;
			
			T->SW_kid = new QuadTreeNode();
			T->SW_kid->rowOffset = (T->rowOffset + T->numRows/2);
			T->SW_kid->colOffset = T->colOffset;
			
			T->SE_kid = new QuadTreeNode();
			T->SE_kid->rowOffset = (T->numRows/2 + T->rowOffset);
			T->SE_kid->colOffset = (T->numCols/2 + T->colOffset);
			
			T->NE_kid = new QuadTreeNode();
			T->NE_kid->rowOffset = T->rowOffset;
			T->NE_kid->colOffset = (T->colOffset + T->numCols/2);
	}
	
	void computenumRowsCols(QuadTreeNode *T)
	{
		if(T==NULL || T->numCols <= 1 || T->numRows <= 1)
		{
			return;
		}
			
		T->NW_kid->numRows = T->numRows/2;
		T->NW_kid->numCols = T->numCols/2;
		
		T->SW_kid->numRows = T->numRows/2;
		T->SW_kid->numCols = T->numCols/2;
		
		T->SE_kid->numRows = T->numRows/2;
		T->SE_kid->numCols = T->numCols/2;
		
		T->NE_kid->numRows = T->numRows/2;
		T->NE_kid->numCols = T->numCols/2;
		
		computeMinMaxValue(T->NW_kid);
		computeMinMaxValue(T->SW_kid);
		computeMinMaxValue(T->SE_kid);
		computeMinMaxValue(T->NE_kid);
		computeColor(T->NW_kid);
		computeColor(T->SW_kid);
		computeColor(T->SE_kid);
		computeColor(T->NE_kid);
	}
	
		void computeMinMaxValue(QuadTreeNode *T)
		{
			if(T==NULL)
				return;
			int numZero = 0;
			int numOne = 0;
			for(int i = T->rowOffset; i < (T->numRows + T->rowOffset); i++)
			{
				for(int j = T->colOffset; j < (T->numCols + T->colOffset); j++)
				{
					if(imgAry[i][j] == 1)
					{
						numOne++;
					}
					else
					{
						numZero++;
					}
				}
			}
			if(numZero == 0 && numOne > 0)
			{
				
				T->minVal = 1;
				T->maxVal = 1;
			}
			else if(numZero > 0 && numOne == 0)
			{
				T->minVal = 0;
				T->maxVal = 0;
			}
			else
			{
				T->minVal = 0;
				T->maxVal = 1;
			}					
		}
		
		void computeColor(QuadTreeNode *T)
		{
			if(T==NULL)
				return;
			if(T->minVal != T->maxVal)
			{
				T->color = 2;
			}
			else{
				if(T->minVal == 1)
				{
					T->color = 1;
				}
				if(T->minVal == 0)
				{
					T->color = 0;
				}
			}
		}

			
	void BuildQuadTree(QuadTreeNode* node)
	{
		if(node == NULL || node->color != 2)
		{
			return;
		}
		else 
		{
			computeOffsets(node);
			computenumRowsCols(node);
			BuildQuadTree(node->NW_kid);
			BuildQuadTree(node->SW_kid);
			BuildQuadTree(node->SE_kid);
			BuildQuadTree(node->NE_kid);
		}
	}
	
	void printQuadTree(QuadTreeNode* node, ofstream &myOut)
	{
		if(node == NULL)
		{
			return;
		}
		else
		{
			QuadTreeNode *temp = new QuadTreeNode();
			temp->printNode(node, myOut);
			printQuadTree(node->NW_kid, myOut);
			printQuadTree(node->SW_kid, myOut);
			printQuadTree(node->SE_kid, myOut);
			printQuadTree(node->NE_kid, myOut);
		}
	}	
		
};

int main(int argc, char* argv[])
{
	Image *inFile = new Image(argv[1], argv[3]);
	ofstream myOut1;
	myOut1.open(argv[2]);
	
	QuadTreeNode *root = new QuadTreeNode();
	root->imgAry = inFile->imgAry;
	root->numRows = inFile->numRows;
	root->numCols = inFile->numCols;
	root->minVal = inFile->minVal;
	root->maxVal = inFile->maxVal;
	root->rowOffset = 0;
	root->colOffset = 0;
	
	QuadTree *myTree = new QuadTree(root, inFile->imgAry);
	myTree->rootNode->numRows = inFile->numRows;
	myTree->rootNode->numCols = inFile->numCols;
	myTree->BuildQuadTree(myTree->rootNode);
	myTree->printQuadTree(myTree->rootNode, myOut1);
	myOut1.close();
	
	return 0;
}
