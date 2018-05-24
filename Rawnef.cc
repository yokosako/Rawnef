#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(void)
  
{
  
  double ab=394.8484497;
  double bb=0.000173119;
  double ag=466.8867054;
  double bg=0.000172106;
  double ar=283.7717951;
  double br=0.000132766;
  
  //double ar=135.8838709,br=0.000219265,ag=325.2598783,bg=0.000223718,ab=301.632707,bb=0.000215264;
  double kr=0,kg=0,kb=0,kall=0;
  int a1;
  cin >> a1;
  //cin >> tara;
  int a2=0,a3=0;
  Mat tmp_img;

  if(a1==1){
    tmp_img = imread("/home/aws/log_data/triming/DSC_6764.JPG", 1);
    a2=6764;
    a3=6781;
    
  }

  if(a1==2){
    //tmp_img = imread("/home/aws/log_data/triming/5803.jpg", 1);
    tmp_img = imread("/home/aws/log_data/triming/DSC_6795.JPG", 1);
    //tmp_img = imread("/home/aws/log_data/triming/b.jpg", 1);
    a2=6795;
    a3=6812;
  }

  if(a1==3){
    tmp_img = imread("/home/aws/log_data/triming/DSC_5809.JPG", 1);
    //tmp_img = imread("/home/aws/log_data/triming/c.jpg", 1);
    a2=5809;
    a3=5830;
  }

    if(a1==4){
    tmp_img = imread("/home/aws/log_data/triming/DSC_7158.jpg", 1);
    //tmp_img = imread("/home/aws/log_data/triming/c.jpg", 1);
    a2=7158;
    a3=7206;
    }

    if(a1==5){
      tmp_img = imread("/home/aws/log_data/triming/DSC_7213.jpg", 1);
      //tmp_img = imread("/home/aws/log_data/triming/c.jpg", 1);
      a2=7213;
      a3=7237;
    }

    if(a1==6){
      tmp_img = imread("/home/aws/log_data/triming/DSC_7757.JPG", 1);
      //tmp_img = imread("/home/aws/log_data/triming/c.jpg", 1);
      a2=7757;
      a3=7769;
    }

    if(a1==7){
      tmp_img = imread("/home/aws/log_data/triming/DSC_7770.JPG", 1);
      //tmp_img = imread("/home/aws/log_data/triming/c.jpg", 1);
      a2=7770;
      a3=7799;
    }

  char FILEname[1024];
  sprintf(FILEname,"/home/aws/log_data/ecef/kyori%d.txt",a1);
  ifstream ifs(FILEname);

  //resize()
  
  char str[256];
  double kyori[100];
  int tako=1;
  int gt=0,bt=0,rt=0;
  /*if(ifs.fail()){
    cerr << "failed" <<endl;
    return -1;
    }*/

  while(ifs.getline(str, 256 - 1)){
    kyori[tako]=atof(str);
    //cout << kyori[tako] << endl;
    tako++;
  }

  //cout << kyori[1] << endl;
  //double tate1=kyori[20]/kyori[1];
  //cout << tate << endl;
  //resize(tmp_img,tmp_img,Size(),kyori[20]/kyori[1],kyori[20]/kyori[1]);

  
  //double tate2=kyori[10]/kyori[1];
  //resize(tmp_img,tmp_img,Size(),(tate2),(tate2));
  
  Mat nef(4924, 7424, CV_16SC1);
  double B,G,R;
  B=0;
  G=0;
  R=0;
  FILE *fp;
  FILE *fp2;
  unsigned char * buf = nef.data;
  int  i, size;
  long lSize;
  char fname[1024];
  char fname2[1024];
  int num=1;
  kyori[0]=kyori[1];
  //cout << kyori[2]<<endl;
  for(int k=a2;k<=a3;k++){
    
    resize(tmp_img,tmp_img,Size(),(kyori[num-1]/kyori[num]),(kyori[num-1]/kyori[num]));
    
    sprintf(fname,"/home/aws/log_data/1116/%d/DSC_%d.NEF",a1,k);
    sprintf(fname2,"/home/aws/log_data/1116/%d/DSC_%d.JPG",a1,k);
    
    fp = fopen(fname, "rb" );
    fp2 = fopen(fname2,"rb");
    if( fp == NULL )continue;
    if( fp2 == NULL )continue;
    Mat search_img = imread(fname2, 1);
    Mat result_img;
    Mat se_img = search_img.clone();

    
    
    matchTemplate(search_img, tmp_img, result_img, CV_TM_CCOEFF_NORMED);
    //matchTemplate(search_img, tmp_img, result_img, CV_TM_CCORR_NORMED);
    
    Rect roi_rect(0, 0, tmp_img.cols, tmp_img.rows);
    Point max_pt;
    double maxVal;
    minMaxLoc(result_img, NULL, &maxVal, NULL, &max_pt);
    roi_rect.x = max_pt.x;
    roi_rect.y = max_pt.y;
    

    
    Mat cut(search_img, Rect(max_pt.x, max_pt.y, tmp_img.cols,tmp_img.rows));
    tmp_img = cut;
    sprintf(fname2,"/home/aws/log_data/triming/%d.jpg",k);
    imwrite(fname2,tmp_img);
    
    fseek(fp,0,SEEK_END);
    lSize = ftell(fp);
    rewind(fp);
    fseek(fp,lSize-(14848*4924),SEEK_SET);
    size = fread( buf, sizeof( unsigned char ), 14848*4924, fp );
    //imshow("result",result_img);
    
    for(int a=roi_rect.y+2*(roi_rect.height)/5;a<roi_rect.y+3*(roi_rect.height)/5;a++){
      for(int b=(roi_rect.x+2*(roi_rect.width)/5)*2;b<(roi_rect.x+3*(roi_rect.width)/5)*2;b+=2){
	if(a%2==0 && b%4==0){
	  R += buf[a*14848+b]*256+buf[a*14848+b+1];
	  rt++;
	}else if(a%2==1 && b%4==2){
	  B += buf[a*14848+b]*256+buf[a*14848+b+1];
	  bt++;
	}else{
	  G += buf[a*14848+b]*256+buf[a*14848+b+1];
	  gt++;
	}
	
      }
    }
    B = B/bt;
    R = R/rt;
    G = G/gt;
    //printf("%d,%d,%d\n",bt,rt,gt);
    printf("%f,%f,%f\n",B,G,R);
    kr=(1/br)*log(R/ar);
    kg=(1/bg)*log(G/ag);
    kb=(1/bb)*log(B/ab);
    kall=(kr+kg+kb)/3;
    //printf("blue:%fm,green:%fm,red:%fm,real:%fm\n",kb,kg,kr,kyori[num]);
    //printf("%d:about%fm,real%fm\n",k,kall,kyori[num]);
    //printf("%f,%f,%f,%f\n",kr,kg,kb,kall);
    //if(kyori[num+1]<2000)break;
    
    bt=0;
    rt=0;
    gt=0;
    num++;
    //ftw.putText(se_img, kall, roi_rect, FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 10, CV_AA);
    rectangle(se_img, roi_rect, Scalar(0, 0, 255),-1);
    resize(se_img,se_img,Size(),0.15,0.15);
    imshow("",se_img);
    
    waitKey(50);
  }
  fclose( fp );
  return 0;
}
