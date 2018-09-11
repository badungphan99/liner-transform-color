#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdlib>
#include <vector>

#define input "ex1.jpeg"

using namespace cv;
using namespace std;

void creatlut(Mat imageInput, Mat imageTarget, int num, float valueTable[3][3]){
    vector<int> inputArray[3];
    vector<int> targetArray[3];
    vector<int> xy[2];
    /* B = YXˆT(XXˆT)ˆ(-1)
     * (XXˆT) = arrayXXT
     * (XXˆT)ˆ(-1) = arrayXXTInv
     *  YXˆT = arrayYXT
     *  valueTable = B       */
    int arrayXXT[3][3];
    int arrayYXT[3][3];
    int arrayXXTInv[3][3];

    for(int i = 0; i < num; i++){
        xy[0].push_back(rand() % imageInput.cols + 0);
        xy[1].push_back(rand() % imageInput.rows + 0);
    }

    for(int i = 0;i < xy[0].size(); i++){
        for(int k = 0;k < 3; k++){
            inputArray[k].push_back(imageInput.at<Vec3b>(xy[0][i],xy[1][i])[k]);
            targetArray[k].push_back(imageTarget.at<Vec3b>(xy[0][i],xy[1][i])[k]);
        }
    }

    for(int i = 0; i < 3 ; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0;k < inputArray[0].size(); k++){
                arrayXXT[i][j] += inputArray[i][k] * inputArray[j][k];
            }
        }
    }

    for(int i = 0; i < 3 ; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0;k < inputArray[0].size(); k++){
                arrayYXT[i][j] += targetArray[i][k] * inputArray[j][k];
            }
        }
    }
    /// Chỗ này cần 1 chương trình con để lấy nghịch đảo của arrayXXT

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                valueTable[i][j] += arrayYXT[i][k] * arrayXXTInv[k][j];
            }
        }
    }
    /// Cần chuẩn hóa lại cái valueTable nữa là xong;

}

int main(){
    Mat image = imread(input,CV_LOAD_IMAGE_COLOR);
    Mat imageInput = image.clone();
    imshow("original image",image);
    float valueTable[3][3];
    float mask[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 0.6}};
    for(int x = 0; x < image.cols;x++){
        for(int y = 0; y < image.rows;y++){
            for(int i=0;i<3;i++){
                float c = 0;
                for(int j=0;j<3;j++){
                    c += mask[i][j] * image.at<Vec3b>(x,y)[j];
                }
                imageInput.at<Vec3b>(x,y)[i] = (uchar)c;
            }
        }
    }

    imshow("image input", imageInput);

    creatlut(imageInput,image,3,valueTable);

    for(int x = 0; x < image.cols;x++){
        for(int y = 0; y < image.rows;y++){
            for(int i=0;i<3;i++){
                float c = 0;
                for(int j=0;j<3;j++){
                    c += valueTable[i][j] * image.at<Vec3b>(x,y)[j];
                }
                image.at<Vec3b>(x,y)[i] = (uchar)c;
            }
        }
    }

    imshow("image output",image);
    waitKey(0);
}

