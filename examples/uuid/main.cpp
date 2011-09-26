/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <iostream>
#include <vector>
#include <string.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

void print_uuid_info(boost::uuids::uuid& uuid, std::string description);
void print_uuid_info(boost::uuids::uuid& uuid);

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    boost::uuids::nil_generator gen;
    boost::uuids::string_generator str_gen;
    boost::uuids::random_generator random_gen;
    boost::uuids::uuid dns_namespace_uuid; // initialize to {6ba7b810-9dad-11d1-80b4-00c04fd430c8}
    boost::uuids::name_generator name_gen(dns_namespace_uuid);

    boost::uuids::uuid u0 = gen();
    boost::uuids::uuid u1 = str_gen("{01234567-89ab-cdef-0123-456789abcdef}");
    boost::uuids::uuid u2 = str_gen(L"01234567-89ab-cdef-0123-456789abcdef");
    boost::uuids::uuid u3 = str_gen(std::string("0123456789abcdef0123456789abcdef"));
    boost::uuids::uuid u4 = str_gen(std::wstring(L"01234567-89ab-cdef-0123-456789abcdef"));
    boost::uuids::uuid u5 = random_gen();
    boost::uuids::uuid u6 = name_gen("boost.org");

    print_uuid_info(u0, std::string("Nill generator"));
    print_uuid_info(u1, std::string("String generator"));
    print_uuid_info(u2, std::string("String generator"));
    print_uuid_info(u3, std::string("String generator"));
    print_uuid_info(u4, std::string("String generator"));
    print_uuid_info(u5, std::string("Random generator"));
    print_uuid_info(u6, std::string("SHA1 generator"));

    std::cout << std::endl;

    for(int i = 1; i < 8; ++i) {
        std::cout << "STRING UUID " << i << " :  "
                  << boost::uuids::to_string(random_gen()) << std::endl;
    }

    return 1;
}

void print_uuid_info(boost::uuids::uuid &uuid, std::string description)
{
    std::cout << description << ":  " << "VERSION UUID:  " << uuid.version()
              << "   SIZE UUID:  " << uuid.size()
              << "   VARIANT UUID:  " << uuid.variant() << std::endl;
}

void print_uuid_info(boost::uuids::uuid& uuid)
{
    std::string s("");
    print_uuid_info(uuid, s);
}
