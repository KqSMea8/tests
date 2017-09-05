FROM paddlepaddle/paddle:latest-dev

RUN apt-get -y install ctags build-essential cmake python-dev llvm   python3-dev \
    man manpages  manpages-de manpages-de-dev manpages-dev lrzsz cscope gdb

RUN cd /root && \
    git clone https://github.com/vim/vim.git && \
    cd vim && \
    ./configure --with-features=huge \
            --enable-multibyte \
            --enable-rubyinterp=yes \
            --enable-pythoninterp=yes \
            --with-python-config-dir=/usr/lib/python2.7/config \
            --enable-perlinterp=yes \
            --enable-luainterp=yes \
            --enable-gui=gtk2 --enable-cscope --prefix=/usr && \
    make -j 8 && make install
            
RUN cd /root && \
    wget --quiet https://raw.githubusercontent.com/gongweibao/tests/develop/paddle/.vimrc && \
    git clone https://github.com/VundleVim/Vundle.vim.git .vim/bundle/Vundle.vim

RUN cd /root && \
    git clone https://github.com/Valloric/YouCompleteMe.git .vim/bundle/YouCompleteMe && \
    cd .vim/bundle/YouCompleteMe && \
    git submodule update --init --recursive && \
    ./install.py --gocode-completer --clang-completer

#RUN mkdir -p /root/go/src/github.com/PaddlePaddle 

RUN vim +PluginInstall +qall
#RUN vim +GoInstallBinaries +qall

RUN echo "alias vi=vim" >> /root/.bashrc
RUN echo "export GOROOT=/usr/local/go" >> /root/.bashrc
RUN echo "export GOPATH=/root/go" >> /root/.bashrc
RUN echo "export PATH=${PATH}:${GOPATH}/bin" >> /root/.bashrc

RUN git config --global user.email "weibao.gong@gmail.com" && \
    git config --global user.name "gongweibao"

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
