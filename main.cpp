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
    float arrayXXT[3][3];
    float arrayYXT[3][3];
    float arrayXXTInv[3][3];
    float determinant = 0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            arrayXXT[i][j] = 0;
            arrayYXT[i][j] = 0;
            arrayXXTInv[i][j] = 0;
            valueTable[i][j] = 0;
        }
    }

    for(int i = 0; i < num; i++){
        xy[0].push_back(rand() % imageInput.cols + 0);
        xy[1].push_back(rand() % imageInput.rows + 0);
    }
    /// Tạo hai ma trận X Y theo công thức
    for(int i = 0;i < xy[0].size(); i++){
        for(int k = 0;k < 3; k++){
            inputArray[k].push_back(imageInput.at<Vec3b>(xy[0][i],xy[1][i])[k]);
            targetArray[k].push_back(imageTarget.at<Vec3b>(xy[0][i],xy[1][i])[k]);
        }
    }

    ///Code ngu nên phải tính từng bước một vậy

    for(int i = 0; i < 3 ; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0;k < inputArray[0].size(); k++){
                arrayXXT[i][j] += inputArray[i][k] * inputArray[j][k];
            }
        }
    }


    /// Đây là chỗ tính det

    for(int i = 0; i < 3; i++) {
        determinant = determinant + (arrayXXT[0][i] * (arrayXXT[1][(i + 1) % 3] * arrayXXT[2][(i + 2) % 3] -
                                                  arrayXXT[1][(i + 2) % 3] * arrayXXT[2][(i + 1) % 3]));
    }
    /// Nghịch đảo ma trận XXˆT

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            arrayXXTInv[i][j] = ((arrayXXT[(j+1)%3][(i+1)%3] * arrayXXT[(j+2)%3][(i+2)%3]) -
                    (arrayXXT[(j+1)%3][(i+2)%3] * arrayXXT[(j+2)%3][(i+1)%3]))/ determinant;
        }
    }
    /// Tính ma trận YXˆT

    for(int i = 0; i < 3 ; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0;k < inputArray[0].size(); k++){
                arrayYXT[i][j] += targetArray[i][k] * inputArray[j][k];
            }
        }
    }
    /// Tính ma trận giá trị 3x3 để tính ánh xạ ngược lại

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                valueTable[i][j] += arrayYXT[i][k] * arrayXXTInv[k][j];
            }
        }
    }
}

int main(){
    /// Đọc ảnh vào
    Mat image = imread(input,CV_LOAD_IMAGE_COLOR);
    Mat imageInput = image.clone();
    imshow("original image",image);
    float valueTable[3][3];
    int mapping = 3;

    /// tạo một cái mask để biến đổi ảnh

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

    /// Đưa ảnh đã bị biến đổi, ảnh gốc, số mapping vào để tính ma trận ánh xạ ngược lại

    creatlut(imageInput,image,mapping,valueTable);

    for(int x = 0; x < image.cols;x++){
        for(int y = 0; y < image.rows;y++){
            for(int i=0;i<3;i++){
                float c = 0;
                for(int j=0;j<3;j++){
                    c += valueTable[i][j] * imageInput.at<Vec3b>(x,y)[j];
                }
                image.at<Vec3b>(x,y)[i] = (uchar)c;
            }
        }
    }

    imshow("image output",image);
    waitKey(0);
}