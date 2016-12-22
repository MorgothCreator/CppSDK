/*
 * httpd.h
 *
 *  Created on: Dec 1, 2016
 *      Author: John Smith
 */

#ifndef LIB_APP_HTTPD_HTTPD_H_
#define LIB_APP_HTTPD_HTTPD_H_

namespace GI
{
namespace App
{
class HttpServer {
public:
	HttpServer(unsigned short port);
	~HttpServer();
};
}
}
#endif /* LIB_APP_HTTPD_HTTPD_H_ */
