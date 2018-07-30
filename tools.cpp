#include "tools.h"

#include "opencv2/legacy/legacy.hpp"
//#include "opencv2/nonfree/nonfree.hpp"

//�Զ��ж�ʶ��OpenCV�İ汾�ţ����ݴ���Ӷ�Ӧ�������⣨.lib�ļ����ķ���
#define CV_VERSION_ID       CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif

#pragma comment( lib, cvLIB("core") )
#pragma comment( lib, cvLIB("imgproc") )
#pragma comment( lib, cvLIB("highgui") )

int g_biBitCount = 24;

//���кϲ����£�
cv::Mat mergeRows(cv::Mat A, cv::Mat B)
{
	// cv::CV_ASSERT(A.cols == B.cols&&A.type() == B.type());
	int totalRows = A.rows + B.rows;
	cv::Mat mergedDescriptors(totalRows, A.cols, A.type());
	cv::Mat submat = mergedDescriptors.rowRange(0, A.rows);
	A.copyTo(submat);
	submat = mergedDescriptors.rowRange(A.rows, totalRows);
	B.copyTo(submat);
	return mergedDescriptors;
}

//���кϲ����£�
cv::Mat mergeCols(cv::Mat A, cv::Mat B)
{
	// cv::CV_ASSERT(A.cols == B.cols&&A.type() == B.type());
	int totalCols = A.cols + B.cols;
	cv::Mat mergedDescriptors(A.rows,totalCols, A.type());
	cv::Mat submat = mergedDescriptors.colRange(0, A.cols);
	A.copyTo(submat);
	submat = mergedDescriptors.colRange(A.cols, totalCols);
	B.copyTo(submat);
	return mergedDescriptors;
}


//ͼ��ģ��ƥ��
//һ����ԣ�Դͼ����ģ��ͼ��patch�ߴ�һ���Ļ�������ֱ��ʹ��������ܵ�ͼ�����ƶȲ����ķ�����
//���Դͼ����ģ��ͼ��ߴ粻һ����ͨ����Ҫ���л���ƥ�䴰�ڣ�ɨ�������ͼ������õ�ƥ��patch��
//��OpenCV�ж�Ӧ�ĺ���Ϊ��matchTemplate()������������������ͼ���л�������Ѱ�Ҹ���λ����ģ��ͼ��patch�����ƶȡ����ƶȵ����۱�׼��ƥ�䷽�����У�
//CV_TM_SQDIFF ƽ����ƥ�䷨�����ƶ�Խ�ߣ�ֵԽС����
//CV_TM_CCORR ���ƥ�䷨�����ó˷����������ƶ�Խ��ֵԽ�󣩣�
//CV_TM_CCOEFF ���ϵ��ƥ�䷨��1��ʾ��õ�ƥ�䣬-1��ʾ����ƥ�䣩��
//ͨ��,���ŴӼ򵥵Ĳ���(ƽ����)�������ӵĲ���(���ϵ��),���ǿɻ��Խ��Խ׼ȷ��ƥ��(ͬʱҲ��ζ��Խ��Խ��ļ������). /
//��õİ취�Ƕ�������Щ���ö���һЩ����ʵ��,�Ա�Ϊ�Լ���Ӧ��ѡ��ͬʱ����ٶȺ;��ȵ���ѷ���.//

//��һ���µ������������ƶȻ��߽��о�������ķ�����EMD��Earth Mover��s Distances
//EMD is defined as the minimal cost that must be paid to transform one histograminto the other, where there is a ��ground distance�� between the basic featuresthat are aggregated into the histogram��
//���߱仯������ͼ����ɫֵ��Ư�ƣ�����Ư��û�иı���ɫֱ��ͼ����״����Ư����������ɫֵλ�õı仯���Ӷ����ܵ���ƥ�����ʧЧ����EMD��һ�ֶ���׼�򣬶���������һ��ֱ��ͼת��Ϊ��һ��ֱ��ͼ����״�������ƶ�ֱ��ͼ�Ĳ��֣���ȫ������һ���µ�λ�ã�����������ά�ȵ�ֱ��ͼ�Ͻ������ֶ�����
//��OpenCV������Ӧ�ļ��㷽����cvCalcEMD2()�������opencv֧�ֿ⣬����ֱ��ͼ�������ԭͼ��HSV��ɫ�ռ�ֱ��ͼ֮���EMD���롣

//cv::Point pt;
//Mat image= imread("board.jpg");
//Mat tepl= imread("position.jpg");
//double d = match(image, tepl, &pt,  CV_TM_SQDIFF) //CV_TM_SQDIFF_NORMED  CV_TM_CCORR CV_TM_CCOEFF CV_TM_CCORR_NORMED CV_TM_CCOEFF_NORMED
double match(cv::Mat image, cv::Mat templ, cv::Point &matchLoc, int method)
{
	int result_cols =  image.cols - templ.cols + 1;
	int result_rows = image.rows - templ.rows + 1;

	cv::Mat result = cv::Mat( result_cols, result_rows, CV_32FC1 );
	cv::matchTemplate( image, templ, result, method );
	//cv::normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal, maxVal, matchVal;
	cv::Point minLoc, maxLoc;
	cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	//CV_TM_SQDIFFƽ����ƥ�䷨�����ƶ�Խ�ߣ�ֵԽС��
	//CV_TM_CCOEFF���ϵ��ƥ�䷨��1��ʾ��õ�ƥ�䣬-1��ʾ����ƥ�䣩��
	//CV_TM_CCORR���ƥ�䷨�����ó˷����������ƶ�Խ��ֵԽ�󣩣�
	switch(method)
	{
	case CV_TM_SQDIFF:								//CV_TM_SQDIFFƽ����ƥ�䷨�����ƶ�Խ�ߣ�ֵԽС��
		matchLoc = minLoc;								//
		matchVal = minVal / (templ.cols * templ.cols);	//ȥ��ģ���С��ƥ��ȵ�Ӱ�죺
		break;
	case CV_TM_CCORR:
	case CV_TM_CCOEFF:
		matchLoc = maxLoc;
		matchVal = maxVal / (templ.cols * templ.cols);
		break;
	case CV_TM_SQDIFF_NORMED:		
		matchLoc = minLoc;
		matchVal =  minVal;
		break;
	case CV_TM_CCORR_NORMED:
	case CV_TM_CCOEFF_NORMED:
		matchLoc = maxLoc;
		matchVal =  maxVal;
		break;
	default:
		matchLoc = maxLoc;
		matchVal =  maxVal;
		break;
	}
#ifdef _DEBUG
	// �������ս��
	//rectangle(image, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
	//imshow( "image", image );
	//waitKey(0);
#endif
	return matchVal;
}

void StrToClip(char* pstr) //����FEN����������
{
	if(::OpenClipboard (NULL))//�򿪼�����
	{
		HANDLE hClip;
		char* pBuf;
		::EmptyClipboard();//��ռ�����

		//д������
		hClip=::GlobalAlloc(GMEM_MOVEABLE,strlen(pstr)+1);
		pBuf=(char*)GlobalLock(hClip);
		strcpy_s(pBuf, strlen(pstr)+1, pstr);
		::GlobalUnlock(hClip);//����
		::SetClipboardData(CF_TEXT,hClip);//���ø�ʽ

		//�رռ�����
		::CloseClipboard();
	}
}

void ClipToStr(char* pstr) //�Ӽ����忽����pstr  ��������е�����
{
	//�жϼ���������ݸ�ʽ�Ƿ���Դ���
	if (!IsClipboardFormatAvailable(CF_TEXT))
	{
		return;
	}

	//�򿪼����塣
	if (!OpenClipboard(NULL))
	{
		return;
	}

	//��ȡUNICODE�����ݡ�
	HGLOBAL hMem = GetClipboardData(CF_TEXT);
	if (hMem != NULL)
	{
		//��ȡUNICODE���ַ�����
		LPTSTR lpStr = (LPTSTR)GlobalLock(hMem);
		if (lpStr != NULL)
		{
			//��ʾ�����
			strcpy_s(pstr, strlen(lpStr)+1, lpStr);

			//�ͷ����ڴ档
			GlobalUnlock(hMem);
		}
	}

	//�رռ����塣
	CloseClipboard();
}

//�ҵ����Ӵ��ڲ�ģ���½���ճ�����棬��ʼ����
void start_bh(string fen, bool turn)
{
	HWND bh = FindWindow(NULL,"BHGUI(test) - �����"); 
	if(bh==NULL || !IsWindow(bh))
	{
		return;
		//int iResult = (int)ShellExecute(NULL,"open","c:\\bh\\bh.exe",NULL,NULL,SW_SHOWNORMAL);    //ִ��Ӧ�ó���
		//Sleep(3000);
		//HWND bh = FindWindow(NULL,"BHGUI(test) - �����"); 
	}
	BringWindowToTop(bh);  
	SetForegroundWindow(bh);

	keybd_event(VK_CONTROL, 0, 0, 0);	//Alt Pres
	keybd_event('N', 0, 0, 0);			//Alt Pres
	keybd_event('N', 0, KEYEVENTF_KEYUP, 0);		
	keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);	
	Sleep(1000);        //ͣ��һ��
	std::cout << "���ӿ��¾�!"<< std::endl;

	StrToClip((char*)fen.c_str()); //����FEN����������
	keybd_event(VK_MENU, 0xb8, 0, 0);	//Alt 
	keybd_event('C', 0, 0, 0);			
	keybd_event('C', 0, KEYEVENTF_KEYUP, 0);		
	keybd_event('P', 0, 0, 0);			
	keybd_event('P', 0, KEYEVENTF_KEYUP,  0);		
	Sleep(1000);        //ͣ��һ��
	std::cout << "����ճ������:  "<< fen <<  std::endl;

	if(turn)
	{
		keybd_event('E', 0, 0, 0);			//Alt Pres
		keybd_event('A', 0, 0, 0);			//Alt Pres
		keybd_event('E', 0, KEYEVENTF_KEYUP, 0);		
		keybd_event('A', 0, KEYEVENTF_KEYUP, 0);		
		Sleep(1000);        //ͣ��һ��
		keybd_event(VK_MENU,0xb8, KEYEVENTF_KEYUP,0);		
		std::cout << "���ӿ�ʼ����!"<< std::endl;
	}
}


void hwnd2mat(cv::Mat& dst, HWND hwnd)
{
	//HWND hwnd=GetDesktopWindow();
	HDC hwindowDC,hWndCompatibleDC;
	int height,width,srcheight,srcwidth;
	HBITMAP hBitmap;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(NULL);
	hWndCompatibleDC=CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hWndCompatibleDC,COLORONCOLOR);  

	RECT wndsize;    // get the height and width of the screen
	GetWindowRect(hwnd, &wndsize);

	srcheight = wndsize.bottom-wndsize.top;
	srcwidth = wndsize.right-wndsize.left;
	height = srcheight;  //change this to whatever size you want to resize to
	width = srcwidth;

	// create a bitmap
	hBitmap = CreateCompatibleBitmap( hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;    
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;    
	bi.biBitCount = g_biBitCount;		//�˴�����Ӧ��CV_8UC4ƥ����С�32λʱ����ʶ��ʱ�������������.jpg��ʽ����˴���ʹʹ��32λ��ȣ����̺�Ҳ��24λ�ģ�win7Ĭ��jpgλ��24λ��PNG32λ����
	bi.biCompression = BI_RGB;    
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;    
	bi.biYPelsPerMeter = 0;    
	bi.biClrUsed = 0;    
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hWndCompatibleDC, hBitmap);
	// copy from the window device context to the bitmap device context
	StretchBlt( hWndCompatibleDC, 0,0, width, height, hwindowDC, wndsize.left, wndsize.top, srcwidth, srcheight, SRCCOPY);	//change SRCCOPY to NOTSRCCOPY for wacky colors !
	
	Mat src;
	src.create(height,width, g_biBitCount==24? CV_8UC3:CV_8UC4);

	GetDIBits(hWndCompatibleDC, hBitmap, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);			//copy from hWndCompatibleDC to hBitmap

	// avoid memory leak
	DeleteObject (hBitmap); DeleteDC(hWndCompatibleDC); ReleaseDC(hwnd, hwindowDC);

	src.copyTo(dst);
}

//�����ʽ�����赲���ڣ��ο�screenCapture.cpp
void hwnd3mat(cv::Mat& dst, HWND hwnd)
{
	//HWND hwnd=GetDesktopWindow();

	HDC hwindowDC,hWndCompatibleDC;

	int height,width,srcheight,srcwidth;
	HBITMAP hBitmap;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC=GetDC(hwnd);
	hWndCompatibleDC=CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hWndCompatibleDC,COLORONCOLOR);  

	RECT wndsize;    // get the height and width of the screen
	GetClientRect(hwnd, &wndsize);

	srcheight = wndsize.bottom;
	srcwidth = wndsize.right;
	height = wndsize.bottom;  //change this to whatever size you want to resize to
	width = wndsize.right;

	src.create(height,width,CV_8UC4);
	//src.create(height,width,CV_16UC4);


	// create a bitmap
	hBitmap = CreateCompatibleBitmap( hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;    
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;    
	bi.biBitCount = 32;    
	bi.biCompression = BI_RGB;    
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;    
	bi.biYPelsPerMeter = 0;    
	bi.biClrUsed = 0;    
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hWndCompatibleDC, hBitmap);
	// copy from the window device context to the bitmap device context
	StretchBlt( hWndCompatibleDC, 0,0, width, height, hwindowDC, 0, 0,srcwidth,srcheight, SRCCOPY);	//change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hWndCompatibleDC,hBitmap,0,height,src.data,(BITMAPINFO *)&bi,DIB_RGB_COLORS);			//copy from hWndCompatibleDC to hBitmap
	
	// avoid memory leak
	DeleteObject (hBitmap); DeleteDC(hWndCompatibleDC); ReleaseDC(hwnd, hwindowDC);
}

void hwnd5mat(cv::Mat& dst, HWND hwnd)//�������
{
	RECT wndsize;						 // get the height and width of the hwnd
	GetWindowRect(hwnd, &wndsize);

	int wndHeight =wndsize.bottom-wndsize.top;
	int wndWidth = wndsize.right-wndsize.left;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);		//�õ���Ļ�ķֱ��ʵ�x
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);		//�õ���Ļ�ֱ��ʵ�y

	HDC hDesktopDC = GetDC( GetDesktopWindow() );			//�õ���Ļ��dc
	HDC hDesktopCompatibleDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hBitmap =CreateCompatibleBitmap(hDesktopDC,wndWidth,wndHeight);	
    SelectObject(hDesktopCompatibleDC,hBitmap); 
	
	StretchBlt( hDesktopCompatibleDC, 0, 0, wndWidth, wndHeight, hDesktopDC, wndsize.left, wndsize.top, wndWidth, wndHeight, SRCCOPY);	//��1�ֿ�������
	//BitBlt(hDesktopCompatibleDC, 0, 0, wndWidth,wndHeight, hDesktopDC, wndsize.left, wndsize.top,SRCCOPY);								//��2�ֿ�������

	Mat src(wndHeight, wndWidth, CV_8UC4);
	
	//��1�ֻ�ȡλͼ���ݵķ���, �ȵ�2�ּ��
	GetBitmapBits(hBitmap, wndWidth*wndHeight*4, src.data);	
	
	//��2�ֻ�ȡλͼ���ݵķ���
	//BITMAPINFO bi;
	//ZeroMemory(&bi, sizeof(BITMAPINFO));
	//bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//bi.bmiHeader.biWidth = wndWidth;
	//bi.bmiHeader.biHeight = -wndHeight;		//negative so (0,0) is at top left
	//bi.bmiHeader.biPlanes = 1;
	//bi.bmiHeader.biBitCount = 32;			//ע����CV_8UC4�������������
	//bi.bmiHeader.biCompression = BI_RGB;    
	//bi.bmiHeader.biSizeImage = 0;  
	//bi.bmiHeader.biXPelsPerMeter = 0;    
	//bi.bmiHeader.biYPelsPerMeter = 0;    
	//bi.bmiHeader.biClrUsed = 0;    
	//bi.bmiHeader.biClrImportant = 0;
	//GetDIBits(hDesktopCompatibleDC, hBitmap, 0, wndHeight, src.data, &bi, DIB_RGB_COLORS);	

	//��������й©
	DeleteObject (hBitmap); ReleaseDC(NULL, hDesktopDC); ReleaseDC(NULL, hDesktopCompatibleDC);//��������й©

	//���������Ŀ��
	src.copyTo(dst);
}


//-----------------------------------��ShowHelpText( )������-----------------------------
//		 ���������һЩ������Ϣ
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	printf("\n\n\t\t\t   ��ǰʹ�õ�OpenCV�汾Ϊ��" CV_VERSION );
	printf("\n\n  ----------------------------------------------------------------------------\n");
}

int hist(  )
{

	//��1�������ز�ͼ����ʾ
	Mat srcImage;
	srcImage=imread("tt2.jpg");
	imshow( "�ز�ͼ", srcImage );

	system("color 3F");
	ShowHelpText();

	//��2������׼��
	int bins = 256;
	int hist_size[] = {bins};
	float range[] = { 0, 256 };
	const float* ranges[] = { range};
	MatND redHist,grayHist,blueHist;
	int channels_r[] = {0};

	//��3������ֱ��ͼ�ļ��㣨��ɫ�������֣�
	calcHist( &srcImage, 1, channels_r, Mat(), //��ʹ����Ĥ
		redHist, 1, hist_size, ranges,
		true, false );

	//��4������ֱ��ͼ�ļ��㣨��ɫ�������֣�
	int channels_g[] = {1};
	calcHist( &srcImage, 1, channels_g, Mat(), // do not use mask
		grayHist, 1, hist_size, ranges,
		true, // the histogram is uniform
		false );

	//��5������ֱ��ͼ�ļ��㣨��ɫ�������֣�
	int channels_b[] = {2};
	calcHist( &srcImage, 1, channels_b, Mat(), // do not use mask
		blueHist, 1, hist_size, ranges,
		true, // the histogram is uniform
		false );

	//-----------------------���Ƴ���ɫֱ��ͼ------------------------
	//����׼��
	double maxValue_red,maxValue_green,maxValue_blue;
	minMaxLoc(redHist, 0, &maxValue_red, 0, 0);
	minMaxLoc(grayHist, 0, &maxValue_green, 0, 0);
	minMaxLoc(blueHist, 0, &maxValue_blue, 0, 0);
	int scale = 1;
	int histHeight=256;
	Mat histImage = Mat::zeros(histHeight,bins*3, CV_8UC3);

	//��ʽ��ʼ����
	for(int i=0;i<bins;i++)
	{
		//����׼��
		float binValue_red = redHist.at<float>(i); 
		float binValue_green = grayHist.at<float>(i);
		float binValue_blue = blueHist.at<float>(i);
		int intensity_red = cvRound(binValue_red*histHeight/maxValue_red);  //Ҫ���Ƶĸ߶�
		int intensity_green = cvRound(binValue_green*histHeight/maxValue_green);  //Ҫ���Ƶĸ߶�
		int intensity_blue = cvRound(binValue_blue*histHeight/maxValue_blue);  //Ҫ���Ƶĸ߶�

		//���ƺ�ɫ������ֱ��ͼ
		rectangle(histImage,Point(i*scale,histHeight-1),
			Point((i+1)*scale - 1, histHeight - intensity_red),
			CV_RGB(255,0,0));

		//������ɫ������ֱ��ͼ
		rectangle(histImage,Point((i+bins)*scale,histHeight-1),
			Point((i+bins+1)*scale - 1, histHeight - intensity_green),
			CV_RGB(0,255,0));

		//������ɫ������ֱ��ͼ
		rectangle(histImage,Point((i+bins*2)*scale,histHeight-1),
			Point((i+bins*2+1)*scale - 1, histHeight - intensity_blue),
			CV_RGB(0,0,255));

	}

	//�ڴ�������ʾ�����ƺõ�ֱ��ͼ
	imshow( "ͼ���RGBֱ��ͼ", histImage );
	waitKey(0);
	return 0;
}


int hist1()
{
	//��1������ԭͼ����ʾ
	Mat srcImage = imread("tt2.jpg", 0);
	imshow("ԭͼ",srcImage);
	if(!srcImage.data) {cout << "fail to load image" << endl; 	return 0;}

	system("color 1F");
	ShowHelpText();

	//��2���������
	MatND dstHist;       // ��cv����CvHistogram *hist = cvCreateHist
	int dims = 1;
	float hranges[] = {0, 255};
	const float *ranges[] = {hranges};   // ������ҪΪconst����
	int size = 256;
	int channels = 0;

	//��3������ͼ���ֱ��ͼ
	calcHist(&srcImage, 1, &channels, Mat(), dstHist, dims, &size, ranges);    // cv ����cvCalcHist
	int scale = 1;

	Mat dstImage(size * scale, size, CV_8U, Scalar(0));
	//��4����ȡ���ֵ����Сֵ
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist,&minValue, &maxValue, 0, 0);  //  ��cv���õ���cvGetMinMaxHistValue

	//��5�����Ƴ�ֱ��ͼ
	int hpt = saturate_cast<int>(0.9 * size);
	for(int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);           //   ע��hist����float����    ����OpenCV1.0������cvQueryHistValue_1D
		int realValue = saturate_cast<int>(binValue * hpt/maxValue);
		rectangle(dstImage,Point(i*scale, size - 1), Point((i+1)*scale - 1, size - realValue), Scalar(255));
	}
	imshow("һάֱ��ͼ", dstImage);
	waitKey(0);
	return 0;
}

int hist_hsv( )
{

	//��1������Դͼ��ת��ΪHSV��ɫģ��
	Mat srcImage, hsvImage;
	srcImage=imread("tt2.jpg");
	cvtColor(srcImage,hsvImage, CV_BGR2HSV);

	system("color 2F");
	ShowHelpText();

	//��2������׼��
	//��ɫ������Ϊ30���ȼ��������Ͷ�����Ϊ32���ȼ�
	int hueBinNum = 30;//ɫ����ֱ��ͼֱ������
	int saturationBinNum = 32;//���Ͷȵ�ֱ��ͼֱ������
	int histSize[ ] = {hueBinNum, saturationBinNum};
	// ����ɫ���ı仯��ΧΪ0��179
	float hueRanges[] = { 0, 180 };
	//���履�Ͷȵı仯��ΧΪ0���ڡ��ס��ң���255����������ɫ��
	float saturationRanges[] = { 0, 256 };
	const float* ranges[] = { hueRanges, saturationRanges };
	MatND dstHist;
	//����׼����calcHist�����н������0ͨ���͵�1ͨ����ֱ��ͼ
	int channels[] = {0, 1};

	//��3����ʽ����calcHist������ֱ��ͼ����
	calcHist( &hsvImage,//���������
		1, //�������Ϊ1
		channels,//ͨ������
		Mat(), //��ʹ����Ĥ
		dstHist, //�����Ŀ��ֱ��ͼ
		2, //��Ҫ�����ֱ��ͼ��ά��Ϊ2
		histSize, //���ÿ��ά�ȵ�ֱ��ͼ�ߴ������
		ranges,//ÿһά��ֵ��ȡֵ��Χ����
		true, // ָʾֱ��ͼ�Ƿ���ȵı�ʶ����true��ʾ���ȵ�ֱ��ͼ
		false );//�ۼƱ�ʶ����false��ʾֱ��ͼ�����ý׶λᱻ����

	//��4��Ϊ����ֱ��ͼ׼������
	double maxValue=0;//���ֵ
	minMaxLoc(dstHist, 0, &maxValue, 0, 0);//����������������ȫ����Сֵ�����ֵ����maxValue��
	int scale = 10;
	Mat histImg = Mat::zeros(saturationBinNum*scale, hueBinNum*10, CV_8UC3);

	//��5��˫��ѭ��������ֱ��ͼ����
	for( int hue = 0; hue < hueBinNum; hue++ )
		for( int saturation = 0; saturation < saturationBinNum; saturation++ )
		{
			float binValue = dstHist.at<float>(hue, saturation);//ֱ��ͼ����ֵ
			int intensity = cvRound(binValue*255/maxValue);//ǿ��

			//��ʽ���л���
			rectangle( histImg, Point(hue*scale, saturation*scale),
				Point( (hue+1)*scale - 1, (saturation+1)*scale - 1),
				Scalar::all(intensity),
				CV_FILLED );
		}

		//��6����ʾЧ��ͼ
		imshow( "�ز�ͼ", srcImage );
		imshow( "H-S ֱ��ͼ", histImg );

		waitKey();
		return 0;
}

double hist_comp(Mat srcImage_base, Mat srcImage_test1)
{
	Mat hsvImage_base;
	Mat hsvImage_test1;

	//srcImage_base = imread( "1.jpg",1 );
	//srcImage_test1 = imread( "2.jpg", 1 );

	//��ͼ����BGRɫ�ʿռ�ת���� HSVɫ�ʿռ�
	cvtColor( srcImage_base, hsvImage_base, CV_BGR2HSV );
	cvtColor( srcImage_test1, hsvImage_test1, CV_BGR2HSV );

	//��ʼ������ֱ��ͼ��Ҫ��ʵ��
	int h_bins = 50;	int s_bins = 60;	
	int histSize[] = { h_bins, s_bins };
	float h_ranges[] = { 0, 256 };		// hue��ȡֵ��Χ��0��256
	float s_ranges[] = { 0, 180 };		//saturationȡֵ��Χ��0��180
	const float* ranges[] = { h_ranges, s_ranges };
	int channels[] = { 0, 1 };// ʹ�õ�0�͵�1ͨ��

	//��������ֱ��ͼ�� MatND ���ʵ��:
	MatND baseHist;
	MatND testHist1;

	//�����׼ͼ�����Ų���ͼ�񣬰����׼ͼ���HSVֱ��ͼ:
	calcHist( &hsvImage_base, 1, channels, Mat(), baseHist, 2, histSize, ranges, true, false );
	normalize( baseHist, baseHist, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist( &hsvImage_test1, 1, channels, Mat(), testHist1, 2, histSize, ranges, true, false );
	normalize( testHist1, testHist1, 0, 1, NORM_MINMAX, -1, Mat() );
	
	double base_test1 = compareHist( baseHist, testHist1, CV_COMP_CORREL );			//int compare_method = 0~3

	return base_test1;
}