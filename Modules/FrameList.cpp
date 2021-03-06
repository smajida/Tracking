#include "FrameList.h"

FrameList::FrameList(int framesToKeep)
{
	maxFrames = framesToKeep;
	infoDisplayMatrix = Mat(480,720, CV_8UC3);
}


void FrameList::open(std::string path)
{
	if ( source.open(path))
	{
		frameAmount = (int)source.get(CV_CAP_PROP_FRAME_COUNT);
		cout << "number of frames: " << frameAmount << endl;
		frameRate = (int)source.get(CV_CAP_PROP_FPS);
		cout << "FPS: " << frameRate << endl;
		currentFrameNumber = 0;
		movieSize = Size(source.get(CV_CAP_PROP_FRAME_WIDTH), source.get(CV_CAP_PROP_FRAME_HEIGHT));

	}
	else
	{
		cout << "unable to open file." << endl;
	}
}

	
Frame & FrameList::getLatestFrame()
{
	return oldFrames.front();
}

list<Frame> & FrameList::getFrames()
{
	return oldFrames;
}


bool FrameList::queryNextFrame()
{
	Mat frameImage;
	if(getCurrentFrameNumber() >= maxFrames)
	{
		oldFrames.pop_back();
	}
	
	if (!source.read(frameImage))
	{
		cout << "eof reached" << endl;
		return false;
	}
	oldFrames.push_front(Frame(frameImage));
	currentFrameNumber++;
	return true;
}


int FrameList::getFrameAmount()
{
	return frameAmount;
}

int FrameList::getCurrentFrameNumber()
{
	return currentFrameNumber;
}

int FrameList::getFrameRate()
{
	return frameRate;
}

vector<Frame> FrameList::toVector()
{
	return vector<Frame>(oldFrames.begin(),oldFrames.end());
}

void FrameList::setTime(std::string name, double time)
{
	getLatestFrame().profileData[name] = time;
}

void FrameList::display(std::string windowID)
{
	int fontFace = CV_FONT_HERSHEY_COMPLEX;
	double fontScale = 0.5;
	int thickness = 1;

	std::string text = "[Frame "+std::to_string(getCurrentFrameNumber())+"("+std::to_string(getFrameAmount())+")]";
	getLatestFrame().drawObjects(cv::Scalar(250, 0, 0, 255));
	putText(getLatestFrame().image, text, Point(5, 15), fontFace, fontScale, Scalar::all(0), thickness, 8);
	imshow( windowID.c_str(), getLatestFrame().image );
	// Record Demo
	getLatestFrame().image.copyTo(getLatestFrame().demoImage(Range(getLatestFrame().image.rows, getLatestFrame().image.rows*2), 
															 Range(getLatestFrame().image.cols, getLatestFrame().image.cols*2)));
}

void FrameList::displayBackground(std::string windowID)
{
	imshow( windowID.c_str(), getLatestFrame().bgModel.pImage );
}

void FrameList::displayBackgroundEstimate(std::string windowID)
{
	imshow( windowID.c_str(), getLatestFrame().bgModel.estimatedBackground );
}

void FrameList::displayBackgroundCertainty(std::string windowID)
{
	imshow( windowID.c_str(), getLatestFrame().bgModel.backgroundCertainty );
}


void FrameList::displayForeground(std::string windowID)
{
	imshow( windowID.c_str(), getLatestFrame().foreground );
}

#define PUTTEXT(x,y,text) putText(infoDisplayMatrix, text, Point(x, y), fontFace, fontScale, Scalar::all(0), thickness, 8);

void FrameList::displayInfo(std::string windowID)
{
	Frame * frame = &getLatestFrame();
	if(oldFrames.size() > 2)
		frame = &(*(++oldFrames.begin()));

	int fontFace = CV_FONT_HERSHEY_COMPLEX;
	double fontScale = 1;
	int thickness = 1;

	infoDisplayMatrix = Scalar::all(200);
	string text;
	int baseline;

	PUTTEXT(5,25,"Profiling:");
	int l = 60;
	for(std::map<std::string, double>::iterator i = frame->profileData.begin(); i != frame->profileData.end(); i++)
	{
		text = "   "+i->first+":";
		while(getTextSize(text, fontFace, fontScale, thickness, &baseline).width < 400)
			text += " ";
		text += std::to_string(i->second);
		PUTTEXT(5,l,text);
		l += 35;
	}

	imshow( windowID.c_str(), infoDisplayMatrix);
}