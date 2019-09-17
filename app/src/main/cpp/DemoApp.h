

#ifndef DEMOAPP_H
#define DEMOAPP_H


#include <jni.h>
#include <string.h>
#include <android/log.h>

#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"


    void DemoAppInit(GLchar* apkFilePath,  GLint width, GLint height );
    void DemoAppDraw( void );
    void DemoAppTouchBegan( GLfloat x, GLfloat y, GLint tap_count );
    void DemoAppTouchMoved( GLfloat x, GLfloat y, GLint tap_count );
    void DemoAppTouchEnded( GLfloat x, GLfloat y, GLint tap_count );

    void DemoAppExit( void );

extern "C"
		{
			JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_Init( JNIEnv *env, jobject obj, jint width, jint height, jstring apkFilePath );
			JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_Draw( JNIEnv *env, jobject obj );
			JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_TouchBegan( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count );
			JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_TouchMoved( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count );
			JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_TouchEnded( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count );


		};

        JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_Init( JNIEnv *env, jobject obj, jint width, jint height, jstring apkFilePath )
		{

            GLchar buf[256];
            strcpy(buf, env->GetStringUTFChars( apkFilePath, NULL ));
            DemoAppInit(buf, width, height );
		}

		JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_Draw( JNIEnv *env, jobject obj )
		{
            DemoAppDraw();
        }

		JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_TouchBegan( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count )
		{
            DemoAppTouchBegan( x, y, tap_count );
        }

		JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_TouchMoved( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count )
		{
            DemoAppTouchMoved( x, y, tap_count );
        }

		JNIEXPORT void JNICALL Java_com_rhino2_navdemo_GL2View_TouchEnded( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count )
		{
            DemoAppTouchEnded( x, y, tap_count );
        }

#endif
