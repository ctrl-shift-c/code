#include <spider.h>

ssl_t *spider_openssl_create(int webfd)
{
	ssl_t *ssl = (ssl_t *)malloc(sizeof(ssl_t));
	SSL_load_error_strings();//初始化ssl错误处理函数
	SSL_library_init();//初始化ssl标准库
	OpenSSL_add_ssl_algorithms();//初始化加密和散列函数

	//创建ssl_ctx 上下文信息
	ssl->sslctx = SSL_CTX_new(SSLv23_method());
	//生成安全套接子
	ssl->sslsocket = SSL_new(ssl->sslctx);
	//通过webfd对ssl进行设置，使其可交互
	SSL_set_fd(ssl->sslsocket, webfd);
	//与https服务器进行安全认证
	SSL_connect(ssl->sslsocket);
	return ssl;
}
