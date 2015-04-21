#!/bin/sh

#  getrepos.sh
#  
#
#  Created by David Haylock on 21/04/2015.
#
#!/bin/sh

repobase="https://github.com";
repos="orgicus/ofxCvPiCam  \
kylemcdonald/ofxCv   \
arturoc/ofxHttpUtils  \
nnnick/Chart.js   \
";

echo "------------------------------";
echo "----> Looking for Addons <----";

# Print the Directory
pwd;

# Cycle through the Repo's Clone if they don't exist
# If they do then Make sure they are upto date
for repo in ${repos};
do
if [ ! -d "${repo}" ]; then
echo "----> Cloning ${repo} <----";
git clone "${repobase}/${repo}.git";
echo "----> Cloned ${repo} <----";
else
echo "----> Pulling ${repo} <----";

# Put us into the addon directory
cd "${repo}";
pwd;

# Pull the latest verison from GitHub
git pull;

# Put us one level up
cd ..;
echo "----> Pulled ${repo} <----";
fi
done
exit;