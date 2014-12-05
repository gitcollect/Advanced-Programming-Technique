#export PYTHONPATH=/usr/local/bin/python2.7/

# Setting PATH for MacPython 2.5
# The orginal version is saved in .bash_profile.pysave
#PATH="/Library/Frameworks/Python.framework/Versions/Current/bin:${PATH}"
#export PATH
cd ~/MBSet

alias qcme="ssh root@q-c.me"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/cuda-4.2/cuda/lib64/:/opt/cuda-4.2/cuda/lib/
##
# Your previous /Users/ex172000/.bash_profile file was backed up as /Users/ex172000/.bash_profile.macports-saved_2014-10-02_at_20:30:26
##

# MacPorts Installer addition on 2014-10-02_at_20:30:26: adding an appropriate PATH variable for use with MacPorts.
#export PATH="/opt/local/bin:/opt/local/sbin:$PATH"
# Finished adapting your PATH environment variable for use with MacPorts.


#if brew list | grep coreutils > /dev/null ; then
#  PATH="$(brew --prefix coreutils)/libexec/gnubin:$PATH"
#  alias ls='ls -F --show-control-chars --color=auto'
#  eval `gdircolors -b $HOME/.dir_colors`
#fi

# The next line updates PATH for the Google Cloud SDK.
#source '/Users/ex172000/google-cloud-sdk/path.bash.inc'

# The next line enables bash completion for gcloud.
#source '/Users/ex172000/google-cloud-sdk/completion.bash.inc'
#export PATH=/usr/local/bin:$PATH
#export PATH=/usr/local/sbin:$PATH
# Added by install_latest_perl_osx.pl
#[ -r /Users/ex172000/.bashrc ] && source /Users/ex172000/.bashrc
