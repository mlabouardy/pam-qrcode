/* Include PAM headers */
#include "util/rapidjson/document.h"
#include <stdio.h>
#include <stdlib.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/_pam_macros.h>
#include <security/_pam_types.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <syslog.h>
#include <zbar.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "util/Parser.h"
#include <vector>
#include "model/Challenge.h"
#include "model/Response.h"

#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

using namespace std;
using namespace zbar;
using namespace cv;

/* PAM entry point for session creation */
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

/* PAM entry point for accounting */
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS; 
}

Response decoder(string content){
   rapidjson::Document d;
	//char chars[] = {'\\'};
	//content.erase (std::remove(content.begin(), content.end(), chars[0]), content.end());
	char *a=new char[content.size()+1];
	a[content.size()]=0;
	memcpy(a,content.c_str(),content.size());
	d.Parse<0>(a);
	Response *response=new Response(d["username"].GetString(),d["pc_name"].GetString(),d["response"].GetInt());
	return *response;
}

/* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY);
    IplImage* frame;
    //bool found=false;	
	 ImageScanner scanner;
	 scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	 const char* user = NULL;
	 int retval = pam_get_user(pamh, &user, NULL);
	 if(retval != PAM_SUCCESS){
		 cout<<"pam_get_user returned error:"<<pam_strerror(pamh,retval)<<endl;
		 return retval;
	 }
	 if (user == NULL || *user == '\0'){
		 cout<<"username not known"<<endl;
		 return PAM_AUTHINFO_UNAVAIL;
	 }
	 string username(user);
	 Parser p;
	 vector<Challenge> challenges=p.parse();
    while (1)  {
        frame = cvQueryFrame(capture);
        cvSaveImage("/etc/pamqrcode/history.png", frame);
        Mat f(frame);
        Mat grey;
        cvtColor(f,grey,CV_BGR2GRAY);
        int width = f.cols;
        int height = f.rows;
        uchar *raw = (uchar *)grey.data;
        // wrap image data
        Image image(width, height, "Y800", raw, width * height);
        // scan the image for barcodes
        int n = scanner.scan(image);
        // extract results
      //  string username(user);
        for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
				string content=symbol->get_data();
			   Response response=decoder(content);
			   int challenge=-1;
				for(unsigned int i=0;i<challenges.size();i++){
						if(challenges[i].getUsername().compare(response.getUsername())==0){
							challenge=challenges[i].getChallenge();
						}
				}
				if(challenge+1==response.getResponse()){
						 int retval = pam_set_item(pamh, PAM_USER, response.getUsername().c_str());
		             cout<<"success"<<endl;
				       cvReleaseCapture(&capture);
   					 cvReleaseImage(&frame);
		             return PAM_SUCCESS;
		         }
		         else{
		             cout<<"fail"<<endl;
						 cvReleaseCapture(&capture);
  						 cvReleaseImage(&frame);
		             return PAM_AUTHINFO_UNAVAIL;
		         }
			  // }
          //  found=true;
        }
        // cvWaitKey(40);
    }
   cvReleaseCapture(&capture);
   cvReleaseImage(&frame);
	return PAM_SUCCESS;
}

/*
PAM entry point for setting user credentials (that is, to actually
establish the authenticated user's credentials to the service provider)
*/
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
    
}

/* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
    
}

