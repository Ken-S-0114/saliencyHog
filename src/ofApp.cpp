#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
  // 画像の読み込み
  inputOfImg.load("cat.jpeg");
  inputOfImg.update();
  
  image = ofxCv::toCv( inputOfImg );
  resize( image, image, cv::Size(), 0.3, 0.3 );
  ofxCv::toOf( image, outputOfImg );
  outputOfImg.update();
  
  // 動画の読み込み
  ofBackground( 255,255,255 );
  ofSetVerticalSync( true );
  player.load( "test.mp4" );
  player.play();
  
}

//--------------------------------------------------------------
void ofApp::update(){
  player.update();
  
  if(player.isFrameNew()){
    Mat mat, mat_gray;
    // Mat変換
    mat = ofxCv::toCv( player ).clone();
    // 白黒加工
    cvtColor( mat.clone(), mat_gray, COLOR_BGR2GRAY );
    
    // // 顕著性マップ(SPECTRAL_RESIDUAL)に変換
    saliencyAlgorithm_SPECTRAL_RESIDUAL->computeSaliency( mat_gray.clone(), saliencyMap );
    //    ofLog()<<"saliencyMap_at : "<<(int)saliencyMap.at<uchar>( 0, 0 );
    
    // アルファチャンネルの正規化を行う
    normalize( saliencyMap.clone(), saliencyMap_norm, 0.0, 255.0, NORM_MINMAX );
    //    ofLog()<<"正規化 : "<<(int)saliencyMap_norm.at<uchar>( 0, 0 );
    
    // Matの型（ビット深度）を変換する
    saliencyMap_norm.convertTo( saliencyMap_conv, CV_8UC3 );
    //    ofLog()<<"Mat_type : "<<(double)saliencyMap_conv.at<double>( 0, 0 );
    
    // 最小と最大の要素値とそれらの位置を求める
    minMaxLoc(saliencyMap_conv, &min_val, &max_val, &min_loc, &max_loc, Mat());
    
    //    ofLog()<<"max_location[x] : "<<max_loc.x;
    //    ofLog()<<"max_location[y] : "<<max_loc.y;
    //    ofLog()<<"min_location[x] : "<<min_loc.x;
    //    ofLog()<<"min_location[y] : "<<min_loc.y;
    
    // 画素値の反転(現状 : 0:黒:顕著性が低い, 255:白:顕著性が高い)
    for( int y = 0; y < saliencyMap_conv.cols; ++y ){
      for( int x = 0; x < saliencyMap_conv.rows; ++x ){
        saliencyMap_conv.at<uchar>( x, y ) = 255 - (int)saliencyMap_conv.at<uchar>( x, y );
//        ofLog()<<"(int)saliencyMap_conv.at<uchar>("<<x<<","<<y<< ") : "<<(int)saliencyMap_conv.at<uchar>( x, y );
      }
    }
    // 疑似カラー（カラーマップ）変換 : (0:赤:顕著性が高い, 255:青:顕著性が低い)
    applyColorMap( saliencyMap_conv.clone(), saliencyMap_color, COLORMAP_JET );
  }
  
}

//--------------------------------------------------------------
void ofApp::draw(){
  
  //  // 出力（動画）
  //  player.draw( 0, 0, 640, 360 );
  //  // 顕著性マップ(SPECTRAL_RESIDUAL)を出力
  //  ofxCv::drawMat( saliencyMap_conv, 0, 360, 640, 360 );
  //  // 顕著性マップ(SPECTRAL_RESIDUAL:カラーマップ)を出力
  //  ofxCv::drawMat( saliencyMap_color, 640, 360, 640, 360 );
  
  //--------------------------------------------------------------
  // 顕著性マップ(SPECTRAL_RESIDUAL:カラーマップ)を出力
  ofxCv::drawMat( saliencyMap_color, 0, 0 );
  
  //--------------------------------------------------------------
  
  // UI画像
  outputOfImg.draw( min_loc.x, min_loc.y );
  // FPS表示
  //  ofDrawBitmapStringHighlight( ofToString(ofGetFrameRate()), 20, 20 );
  
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
