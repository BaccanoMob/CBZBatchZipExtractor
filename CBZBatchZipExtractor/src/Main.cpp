#include "ZIPUtil.h"

int main()
{
	bool running = 1;
	
	while (running)
	{
		PRINTLOG("Select function:\n1) Zip all in Directory to cbz files\n" <<
			"2) Zip all in Directory to cbr files\n3) Convert cbz files to cbr\n4) Convert cbr files to cbz\n" <<
			"5) Extract all zipfiles in Directory");
		char getOp = getOption();

		std::string getDir = getDirectory();

		running = chooseFunction(getOp, getDir);

		PRINTLOG("Want to retry?(y/n)");
		char retry = getOption();
		if (retry == 'y' || retry == 'Y') running = 1;
		else running = 0;
	}
}