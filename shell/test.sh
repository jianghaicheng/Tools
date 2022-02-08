install prerequisites.
ci_popart_dir="$( cd "$( dirname "$0"  )" && pwd  )"
echo -e "current_dir: $ci_popart_dir"
poplar_dir=""

# download poplar used sdk
echo -e "start to download the specified poplar sdk"
cd ..

rm -rf poplar_dir
mkdir poplar_dir 
pushd poplar_dir
  bash ~/get_sdk.sh
  virtualenv temp -p python3.6
  source temp/bin/activate
  tar -zxf poplar_sdk-ubuntu_18_04*
  # 当前目录
  cur_dir="$( cd "$( dirname "$0"  )" && pwd  )"
  sdk_folder=`ls -F |grep "/$"`
  sdk_dir=$cur_dir/$sdk_folder

  echo -e "sdkdir: $sdk_dir"
  poplar_folder=`ls $sdk_dir | grep "poplar"`
  echo "poplardir: $poplar_folder"
  poplar_dir=$sdk_dir/$poplar_folder
  echo -e "poplardir: $poplar_dir"
popd

# start to build popart
echo -e " start to build popart"
pushd `mktemp -d`
  git clone ssh://git@phabricator.sourcevertex.net/diffusion/POPONNX/poponnx-v2.git popart
  pip install -r popart/requirements/dev.txt
  rm -rf popart
popd

#Build PopART
git clone ssh://git@phabricator.sourcevertex.net/diffusion/POPARTVIEW/popart_view.git
cd popart_view
gc-view pull

#Authentication
pushd artifactory_api
  python setup_token.py --credentials-file ~/.artifactory_credentials --api-key-file ~/.artifactory_credentials
popd

delete the popart downloaded by popart_view and use the popart downloaded by CI.
rm -rf popart
ln -s $ci_popart_dir popart

BUILD=build
mkdir -p $BUILD
mkdir -p $BUILD/build/popart/metadata

# default SDK does not include latest changes from poplar and libpva
# POPLAR_INSTALL=$(pwd)/unpacked/poplar/poplar-ubuntu_18_04-2.4.0+1823-de82a7008a/
#cmake "-DCMAKE_BUILD_TYPE=Release" \
#           "-DPOPART_CMAKE_ARGS=-DPOPLAR_INSTALL_DIR=/absolute/path/to/popart_view/unpacked/poplar/poplar-XXX-XXX" \
#             -GNinja .""""
POPLAR_INSTALL=$poplar_dir

cd $BUILD
# (Optional) generate popart depency file $BUILD/ProjectInfo.cmake
../cbt/cbt.py --generate-project-info ..

../cbt/cbt.py ..
#cmake "-DCMAKE_BUILD_TYPE=Debug" \
#  "-DPOPART_CMAKE_ARGS=-DPOPLAR_INSTALL_DIR=$POPLAR_INSTALL" -GNinja .
cmake "-DCMAKE_BUILD_TYPE=Release" \
  "-DPOPART_CMAKE_ARGS=-DPOPLAR_INSTALL_DIR=$POPLAR_INSTALL"
#  "-DPOPART_CMAKE_ARGS=-DENABLED_TEST_VARIANTS=Hw"
cmake --build .
