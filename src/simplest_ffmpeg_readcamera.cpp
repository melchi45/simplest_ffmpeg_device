/**
 * The simplest FFmpeg based AVDevice example (read camera)
 * Simplest FFmpeg Device (Read Camera)
 *
 * Thunder Lei Xiaohua
 * leixiaohua1020@126.com
 * Communication University of China / Digital TV Technology
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * This program implements the acquisition, decoding and display of local camera data. Is based on FFmpeg
 * The simplest example of a libavdevice library. Through this example, you can learn FFmpeg
 * How to use the libavdevice library.
 * This program can read camera data in two ways under Windows:
 * 1.VFW: Video for Windows screen capture device. Note that the input URL is the serial number of the device.
 * From 0 to 9.
 * 2.dshow: Use Directshow. Note that the name of the camera device on the author's machine is
 * "Integrated Camera", you need to change your camera on your computer
 * The name of the equipment.
 * You can use video4linux2 to read the camera device under Linux.
 * You can use avfoundation to read the camera device under MacOS.
 *
 * This software read data from Computer's Camera and play it.
 * It's the simplest example about usage of FFmpeg's libavdevice Library.
 * It's suiltable for the beginner of FFmpeg.
 * This software support 2 methods to read camera in Microsoft Windows:
 * 1.gdigrab: VfW (Video for Windows) capture input device.
 * The filename passed as input is the capture driver number,
 * ranging from 0 to 9.
 * 2.dshow: Use Directshow. Camera's name in author's computer is
 * "Integrated Camera".
 * It use video4linux2 to read Camera in Linux.
 * It use avfoundation to read Camera in MacOS.
 *
 */

#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <SDL2/SDL.h>
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <SDL2/SDL.h>
#ifdef __cplusplus
};
#endif
#endif

// SDL window size
#define WINDOWS_WIDTH	1280
#define WINDOWS_HEIGHT	720

//Output YUV420P 
#define OUTPUT_YUV420P 0
//'1' Use Dshow 
//'0' Use VFW
#define USE_DSHOW 0


//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

int thread_exit=0;

int sfp_refresh_thread(void *opaque)
{
	thread_exit=0;
	while (!thread_exit) {
		SDL_Event event;
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	thread_exit=0;
	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}

//Show Dshow Device
void show_dshow_device(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options,"list_devices","true",0);
	AVInputFormat *iformat = av_find_input_format("dshow");
	printf("========Device Info=============\n");
	avformat_open_input(&pFormatCtx,"video=dummy",iformat,&options);
	printf("================================\n");
}

//Show Dshow Device Option
void show_dshow_device_option(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options,"list_options","true",0);
	AVInputFormat *iformat = av_find_input_format("dshow");
	printf("========Device Option Info======\n");
	avformat_open_input(&pFormatCtx,"video=Integrated Camera",iformat,&options);
	printf("================================\n");
}

//Show VFW Device
void show_vfw_device(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVInputFormat *iformat = av_find_input_format("vfwcap");
	printf("========VFW Device Info======\n");
	avformat_open_input(&pFormatCtx,"list",iformat,NULL);
	printf("=============================\n");
}

//Show AVFoundation Device
void show_avfoundation_device(){
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options,"list_devices","true",0);
    AVInputFormat *iformat = av_find_input_format("avfoundation");
    printf("==AVFoundation Device Info===\n");
    avformat_open_input(&pFormatCtx,"",iformat,&options);
    printf("=============================\n");
}


int main(int argc, char* argv[])
{

	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	Uint8 *yPlane, *uPlane, *vPlane;
	size_t yPlaneSz, uvPlaneSz;
	int uvPitch;
	
	av_register_all();
	//Register Device
	avdevice_register_all();

	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	//Open File
	//char filepath[]="src01_480x272_22.h265";
	//avformat_open_input(&pFormatCtx,filepath,NULL,NULL)

//Windows
#ifdef _WIN32

	//Show Dshow Device
	show_dshow_device();
	//Show Device Options
	show_dshow_device_option();
    //Show VFW Options
    show_vfw_device();

#if USE_DSHOW
	AVInputFormat *ifmt=av_find_input_format("dshow");
	//Set own video device's name
	if(avformat_open_input(&pFormatCtx,"video=Integrated Camera",ifmt,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
#else
	AVInputFormat *ifmt=av_find_input_format("vfwcap");
	if (ifmt == NULL)
	{
		printf("can not find_input_format\n");
		return -1;
	}
	char *dev_name = "0";
	if(avformat_open_input(&pFormatCtx, dev_name,ifmt,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
#endif
#elif defined linux
    //Linux
	AVInputFormat *ifmt=av_find_input_format("video4linux2");
	if(avformat_open_input(&pFormatCtx,"/dev/video0",ifmt,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
#else
    show_avfoundation_device();
    //Mac
    AVInputFormat *ifmt=av_find_input_format("avfoundation");
    //Avfoundation
    //[video]:[audio]
    if(avformat_open_input(&pFormatCtx,"0",ifmt,NULL)!=0){
        printf("Couldn't open input stream.\n");
        return -1;
    }
#endif


	if(avformat_find_stream_info(pFormatCtx,NULL)<0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoindex=i;
			break;
		}
	if(videoindex==-1)
	{
		printf("Couldn't find a video stream.\n");
		return -1;
	}
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
	{
		printf("Could not open codec.\n");
		return -1;
	}
	AVFrame	*pFrame;
//	AVFrame *pFrameYUV;
	pFrame=av_frame_alloc();
//	pFrameYUV=av_frame_alloc();
	//unsigned char *out_buffer=(unsigned char *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	//avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	//SDL----------------------------
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 

	// Create wndow 
	SDL_Window *window = SDL_CreateWindow(
		"Simplest FFmpeg Read Camera",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		pCodecCtx->width, pCodecCtx->height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	// Create renderer 
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// Create texture 
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);
	if (!texture) {
		fprintf(stderr, "SDL: could not create texture - exiting\n");
		exit(1);
	}
	//SDL End------------------------
	int ret, got_picture;

	AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));

#if OUTPUT_YUV420P 
    FILE *fp_yuv=fopen("output.yuv","wb+");  
#endif  

	struct SwsContext *img_convert_ctx;
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
	
	// set up YV12 pixel array (12 bits per pixel)
	yPlaneSz = pCodecCtx->width * pCodecCtx->height;
	uvPlaneSz = pCodecCtx->width * pCodecCtx->height / 4;
	yPlane = (Uint8*)malloc(yPlaneSz);
	uPlane = (Uint8*)malloc(uvPlaneSz);
	vPlane = (Uint8*)malloc(uvPlaneSz);
	if (!yPlane || !uPlane || !vPlane) {
		fprintf(stderr, "Could not allocate pixel buffers - exiting\n");
		exit(1);
	}
	uvPitch = pCodecCtx->width / 2;

	//------------------------------
	SDL_Thread *video_tid = SDL_CreateThread(sfp_refresh_thread,NULL, NULL);
	//Event Loop
	SDL_Event event;

	for (;;) {
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==SFM_REFRESH_EVENT) {
			//------------------------------
			if(av_read_frame(pFormatCtx, packet)>=0){
				if(packet->stream_index==videoindex){
					ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
					if(ret < 0){
						printf("Decode Error.\n");
						return -1;
					}
					if(got_picture){
						AVPicture pict;
						pict.data[0] = yPlane;
						pict.data[1] = uPlane;
						pict.data[2] = vPlane;
						pict.linesize[0] = pCodecCtx->width;
						pict.linesize[1] = uvPitch;
						pict.linesize[2] = uvPitch;

						sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pict.data, pict.linesize);

#if OUTPUT_YUV420P  
						int y_size=pCodecCtx->width*pCodecCtx->height;    
						fwrite(pict->data[0],1,y_size,fp_yuv);    //Y   
						fwrite(pict->data[1],1,y_size/4,fp_yuv);  //U  
						fwrite(pict->data[2],1,y_size/4,fp_yuv);  //V  
#endif  
					   // Lock my texture 
						//SDL_UpdateYUVTexture(texture, NULL, pFrame->data[0], pFrame->linesize[0],
						//	pFrame->data[1], pFrame->linesize[1],
						//	pFrame->data[2], pFrame->linesize[2]);
						SDL_UpdateYUVTexture(
							texture,
							NULL,
							yPlane,
							pCodecCtx->width,
							uPlane,
							uvPitch,
							vPlane,
							uvPitch
						);

						//SDL_Rect texr; texr.x = WINDOWS_WIDTH / 2; texr.y = WINDOWS_HEIGHT / 2; texr.w = width * 2; texr.h = height * 2;
						SDL_Rect texure_rect; texure_rect.x = 0; texure_rect.y = 0; texure_rect.w = pCodecCtx->width; texure_rect.h = pCodecCtx->height;
						SDL_Rect windows_rect; windows_rect.x = 0; windows_rect.y = 0; windows_rect.w = WINDOWS_WIDTH; windows_rect.h = WINDOWS_HEIGHT;

						SDL_RenderClear(renderer);
						SDL_RenderCopy(renderer, texture, &windows_rect, &texure_rect);
						SDL_RenderPresent(renderer);
					}
				}
				if(packet != NULL)
					av_free_packet(packet);
			}else{
				//Exit Thread
				thread_exit=1;
			}
		}else if(event.type==SDL_QUIT){
			thread_exit=1;

			SDL_DestroyTexture(texture);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);

			SDL_Quit();
		}else if(event.type==SFM_BREAK_EVENT){
			break;
		}

	}
	
	sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P 
    fclose(fp_yuv);
#endif 

	//av_free(out_buffer);
//	av_free(pFrameYUV);

	if(pFrame != NULL)
		av_free(pFrame);

	free(yPlane);
	free(uPlane);
	free(vPlane);

	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}

