//
//
//

#ifndef _ZRREADER_H
#define _ZRREADER_H

#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include <vector>


class ZReader
{
    GLchar			filePathname[256];
    GLuint			size;
    GLuint			position;
    GLchar*         buffer;

    GLubyte*  buffertoo;
public:
//constructors
    ZReader(){
        size = 0, position = 0;
        buffer = NULL;
        buffertoo = NULL;
        strcpy(filePathname, "EMPTY");
    }
    ~ZReader(){
        if(buffer != NULL){
            free(buffer);
            buffer = NULL;
        }
        if(buffertoo != NULL){
            free(buffertoo);
            buffertoo = NULL;
        }
    }
    void Init(GLchar* apkFilePath){
        size = 0, position = 0;
        strcpy(filePathname, apkFilePath);
    }
//member functions
    GLuint bufferLen() {return size + 1;}
    GLchar* ZGetBuffer() {
        return &buffer[0];
    }
    GLubyte* RetBufToo(){
        return &buffertoo[0];
    }
    GLshort ZOpen(GLchar* asset_filename, bool relative_path);
    GLshort ZOpenFont(GLchar* asset_filename, bool relative_path);
    GLuint Zread(void *dst, GLuint size );
    void ZClose() {
        size = 0, position = 0;
    }

};

#endif //_ZRREADER_H
