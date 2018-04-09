#ifndef MOVIEINFO_H
#define MOVIEINFO_H
#include <QString>
#include <QDateTime>
typedef enum StereoFormat {
  STEREO_FORMAT_TB = 0,
  STEREO_FORMAT_LR,
  STEREO_FORMAT_MONO,
  STEREO_FORMAT_GUESS,
  STEREO_FORMAT_N
} StereoFormat;

typedef enum InterpolationAlg {
  NEAREST = 0,
  LINEAR = 1,
  CUBIC = 2,
  LANCZOS4 = 4
} InterpolationAlg;
typedef enum Layout {
  LAYOUT_CUBEMAP_32 = 0,
  LAYOUT_CUBEMAP_23_OFFCENTER,
#ifdef FACEBOOK_LAYOUT
  LAYOUT_FB,
#endif
  LAYOUT_EQUIRECT,
  LAYOUT_BARREL,
  LAYOUT_EAC_32,
  LAYOUT_N
} Layout;
typedef enum Encoder {
  x264=0,
  x265,
}Encoder;
typedef enum Muxer{
    avi=0,
    mp4=1,
}Muxer;
typedef struct TransformContext {
    int w=0, h=0;
    int out_map_planes= 0;

    char *w_expr;               ///< width  expression string
    char *h_expr;               ///< height expression string
    char *size_str=NULL;
    int cube_edge_length=0;
    int max_cube_edge_length=960;
    int max_output_h=0;
    int max_output_w=0;
    int input_layout=LAYOUT_EQUIRECT;
    int output_layout=LAYOUT_CUBEMAP_32;
    int input_stereo_format=STEREO_FORMAT_MONO;
    int output_stereo_format=STEREO_FORMAT_MONO;
    int vflip;
    int planes;
    float input_expand_coef =1.01f;
    float expand_coef =1.01f;
    int is_horizontal_offset;
    float fixed_yaw = 0.0;    ///< Yaw (asimuth) angle, degrees
    float fixed_pitch = 0.0;  ///< Pitch (elevation) angle, degrees
    float fixed_roll = 0.0;   ///< Roll (tilt) angle, degrees
    float fixed_cube_offcenter_x; // offcenter projection x
    float fixed_cube_offcenter_y; // offcenter projection y
    float fixed_cube_offcenter_z; // offcenter projection z

                                  // openCV-based transform parameters
    int interpolation_alg=CUBIC;
    float width_scale_factor= 1.0;
    float height_scale_factor;
    int enable_low_pass_filter = 0;
    float kernel_height_scale_factor = 0;
    float min_kernel_half_height= 1.0;
    float max_kernel_half_height=10000.0;
    int enable_multi_threading= 0;
    int num_vertical_segments= 2;
    int num_horizontal_segments= 1;
    int adjust_kernel= 1;
    float kernel_adjust_factor= 1.0;

} TransformContext;
class MovieInfo
{
public:
    MovieInfo();
public:
    QString name,path,output,outputpath,outputfile;
    QDateTime start,end,duration;
    bool projectflag=false,editflag=false,finishflag=false;
    TransformContext setting;
    int row_num,path_index;
    Encoder encoder=x264;
    Muxer muxer=avi;
    bool projectionpreview=false;
};

#endif // MOVIEINFO_H
