#include "ofApp.h"
#define SALIENCY 150000
//--------------------------------------------------------------
void ofApp::setup(){
  
  // 画像の読み込み
  inputOfImg.load("cat.jpeg");
  inputOfImg.update();

  image = ofxCv::toCv( inputOfImg );
  resize( image, image, cv::Size(), 128.0/image.cols, 72.0/image.cols );
  ofxCv::toOf( image, outputOfImg );
  outputOfImg.update();

  // 動画の読み込み
//  ofBackground( 255,255,255 );
//  ofSetVerticalSync( true );
//  player.load( "test.mp4" );
//  player.play();
  
  // カメラの設定
  camWidth = 1280;
  camHeight = 720;
  
  vector<ofVideoDevice> devices = vidGrabber.listDevices();
  
  for(size_t i = 0; i < devices.size(); i++){
    if(devices[i].bAvailable){
      ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
    }else{
      ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
    }
  }
  
  vidGrabber.setDeviceID(0);
  vidGrabber.setDesiredFrameRate(60);
  vidGrabber.initGrabber(camWidth, camHeight);

  ofSetVerticalSync(true);
  
  // 10*10の顕著マップの最小値の場所
  widthMin = 0;
  heightMin = 0;
  
  // 1回目と判定
  firstFrameCheck = true;
  // UIを出した箇所が次のフレームで一定数値以下であればUIを動かさない
  algorithmCheck = true;
}

//--------------------------------------------------------------
void ofApp::update(){
  
//  player.update();
//  if(player.isFrameNew()){
  
  ofBackground(100, 100, 100);
  vidGrabber.update();
  
  if( vidGrabber.isFrameNew() ){
    ofPixels & pixels = vidGrabber.getPixels();

    Mat mat, mat_gray;
    // Mat変換
    mat = ofxCv::toCv( pixels ).clone();
    // 白黒加工
    cvtColor( mat.clone(), mat_gray, COLOR_BGR2GRAY );

    // 顕著性マップ(SPECTRAL_RESIDUAL)に変換
    saliencyAlgorithm_SPECTRAL_RESIDUAL->computeSaliency( mat_gray.clone(), saliencyMap );
    // アルファチャンネルの正規化を行う
    normalize( saliencyMap.clone(), saliencyMap_norm, 0.0, 255.0, NORM_MINMAX );
    // Matの型（ビット深度）を変換する
    saliencyMap_norm.convertTo( saliencyMap_conv, CV_8UC3 );
    
    // 最小と最大の要素値とそれらの位置を求める
//    minMaxLoc(saliencyMap_conv, &min_val, &max_val, &min_loc, &max_loc, Mat());
    
    // updateが2回目以降の場合, if文の中に入る
    if(!firstFrameCheck){
      // 前回の顕著性マップで顕著性が低かった10*10ピクセルのうちの一つ
      cv::Rect roi(widthMin, heightMin, saliencyMap_conv.cols / 10, saliencyMap_conv.rows / 10);
      Mat saliency_roi = saliencyMap_conv(roi);
      int pixels = 0;
      // 10 * 10のうちの一つの画素値
      for( int y = 0; y < saliency_roi.cols; ++y ){
        for( int x = 0; x < saliency_roi.rows; ++x ){
          pixels += (int)saliency_roi.at<uchar>( x, y );
        }
      }
      // UIを出した箇所が次のフレームで一定数値以下であればUIを動かさないフラグを設定
      algorithmCheck = pixels < SALIENCY ? false : true ;
      
    }
    
    // 10*10のうちの画素最小値の場所を取得
    if ( algorithmCheck ){
      // 処理領域を設定
      int height = 0;
      int minPixels = 0;
      
      for( int heightCount = 0; heightCount < 10; ++heightCount ){
        int width = 0;
        for( int widthCount = 0; widthCount < 10; ++widthCount ){
          cv::Rect roi(width, height, saliencyMap_conv.cols / 10, saliencyMap_conv.rows / 10);
          Mat saliency_roi = saliencyMap_conv(roi);
          
          // 10*10のうちの一つの画素値
          int pixels = 0;
          for( int y = 0; y < saliency_roi.cols; ++y ){
            for( int x = 0; x < saliency_roi.rows; ++x ){
              pixels += (int)saliency_roi.at<uchar>( x, y );
            }
          }
          
          // 最小値の値とその場所を更新
          if ( ( heightCount == 0 && widthCount == 0 ) || pixels < minPixels ) {
            minPixels = pixels;
            widthMin = width;
            heightMin = height;
            
          }
          width += saliencyMap_conv.cols / 10;

        }
        height += saliencyMap_conv.rows / 10;
      }
    }
    
    // 画素値の反転（現状 : 0:黒:顕著性が低い, 255:白:顕著性が高い）
    for( int y = 0; y < saliencyMap_conv.cols; ++y ){
      for( int x = 0; x < saliencyMap_conv.rows; ++x ){
        saliencyMap_conv.at<uchar>( x, y ) = 255 - (int)saliencyMap_conv.at<uchar>( x, y );
//        ofLog()<<"(int)saliencyMap_conv.at<uchar>("<<x<<","<<y<< ") : "<<(int)saliencyMap_conv.at<uchar>( x, y );
      }
    }
    // 疑似カラー（カラーマップ）変換 :（0:赤:顕著性が高い, 255:青:顕著性が低い）
    applyColorMap( saliencyMap_conv.clone(), saliencyMap_color, COLORMAP_JET );
    // 初回のチェックをなくす
    firstFrameCheck = false;

  }
  
}

//--------------------------------------------------------------
void ofApp::draw(){
  
//  // 出力（動画）
//  player.draw( 0, 0, 640, 360 );
//  // 出力（カメラ）
//  ofSetHexColor(0xffffff);
//  vidGrabber.draw(0, 0, 640, 360);
//  // 顕著性マップ(SPECTRAL_RESIDUAL)を出力
//  ofxCv::drawMat( saliencyMap_conv, 0, 360, 640, 360 );
//  // 顕著性マップ(SPECTRAL_RESIDUAL:カラーマップ)を出力
//  ofxCv::drawMat( saliencyMap_color, 640, 360, 640, 360 );

  //--------------------------------------------------------------
  // 顕著性マップ(SPECTRAL_RESIDUAL:カラーマップ)を出力
  ofxCv::drawMat( saliencyMap_color, 0, 0 );
  
  //--------------------------------------------------------------
  
  // UI画像
  outputOfImg.draw( widthMin, heightMin );
  // FPS表示
  ofDrawBitmapStringHighlight( ofToString(ofGetFrameRate()), 20, 20 );
  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
  
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
  
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
  
}
