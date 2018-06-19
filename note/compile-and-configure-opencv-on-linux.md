compile opencv and config it in QtCreator

-----------------------------------------

# compile steps
```shell
# put opencv and opencv_contrib into one directory
cd opencv
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=./install -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
make -j6
```

# configuration in QtCreator(.pro)

INCLUDEPATH+=/home/kekliu/opt/opencv-3.3.0/opencv/build_with_contrib/install/include

CONFIG(debug, debug|release): {
LIBS += -L/home/kekliu/opt/opencv-3.3.0/opencv/build_with_contrib/install/lib \
-lopencv_aruco \
-lopencv_bgsegm \
-lopencv_bioinspired \
-lopencv_calib3d \
-lopencv_ccalib \
-lopencv_core \
-lopencv_datasets \
-lopencv_dnn \
-lopencv_dpm \
-lopencv_face \
-lopencv_features2d \
-lopencv_flann \
-lopencv_freetype \
-lopencv_fuzzy \
-lopencv_hdf \
-lopencv_highgui \
-lopencv_imgcodecs \
-lopencv_img_hash \
-lopencv_imgproc \
-lopencv_line_descriptor \
-lopencv_ml \
-lopencv_objdetect \
-lopencv_optflow \
-lopencv_phase_unwrapping \
-lopencv_photo \
-lopencv_plot \
-lopencv_reg \
-lopencv_rgbd \
-lopencv_saliency \
-lopencv_shape \
-lopencv_stereo \
-lopencv_stitching \
-lopencv_structured_light \
-lopencv_superres \
-lopencv_surface_matching \
-lopencv_text \
-lopencv_tracking \
-lopencv_videoio \
-lopencv_video \
-lopencv_videostab \
-lopencv_viz \
-lopencv_xfeatures2d \
-lopencv_ximgproc \
-lopencv_xobjdetect \
-lopencv_xphoto
} else:CONFIG(release, debug|release): {
LIBS += -L/home/kekliu/opt/opencv-3.3.0/opencv/build_with_contrib/install/lib \
-lopencv_aruco \
-lopencv_bgsegm \
-lopencv_bioinspired \
-lopencv_calib3d \
-lopencv_ccalib \
-lopencv_core \
-lopencv_datasets \
-lopencv_dnn \
-lopencv_dpm \
-lopencv_face \
-lopencv_features2d \
-lopencv_flann \
-lopencv_freetype \
-lopencv_fuzzy \
-lopencv_hdf \
-lopencv_highgui \
-lopencv_imgcodecs \
-lopencv_img_hash \
-lopencv_imgproc \
-lopencv_line_descriptor \
-lopencv_ml \
-lopencv_objdetect \
-lopencv_optflow \
-lopencv_phase_unwrapping \
-lopencv_photo \
-lopencv_plot \
-lopencv_reg \
-lopencv_rgbd \
-lopencv_saliency \
-lopencv_shape \
-lopencv_stereo \
-lopencv_stitching \
-lopencv_structured_light \
-lopencv_superres \
-lopencv_surface_matching \
-lopencv_text \
-lopencv_tracking \
-lopencv_videoio \
-lopencv_video \
-lopencv_videostab \
-lopencv_viz \
-lopencv_xfeatures2d \
-lopencv_ximgproc \
-lopencv_xobjdetect \
-lopencv_xphoto
}

