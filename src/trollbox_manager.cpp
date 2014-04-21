/**
 -----BEGIN PGP MESSAGE-----
hQIMA8Hhi7HOnyjIAQ//dmGnnsJBxqGuX8U1iJ/JxEj2IMKUHs48ceYMOsb4bEpO
aq7cCcGHgEgBUJnlIhBDYE58CRfQIRs4yJLfuRGLPgYj0cu2i9hP3ZWnCyM3pb8I
V54CS4FlPw8nksz04DGPI3L1y73sFdNkNY3VjrFATGuXNL5915IRcv0AiGHxrJaD
CJn2Brxhnh5hTA+/hZ9g65O2YsWhxj1WIBEwibiOWYilYGuonL6ZWLwPI/oKxG94
bx98gKfoQPMUPglGHh5KGqIBMeNgEQvjHfZ5kL9SW31Iq0uZ+3D8MSZ3w0EvsfCq
qSSp5bLSEmnGX/a56DyrDPxvqqGozbCLK90MOQVTcJgKJQnFlcQDUbFPZ4xU4zcl
GeSNML0mxfmHOloetCbVIIhavkQ5jV7Fd04emo2duATW/4d4tsw1lk1IFYcy9LGk
ricRF/RTh60y5R5RNtB4VkcnnyM3zki7I/FKNADTaSKAmlVsfhiEkJ1W/2hj0wSb
qRiPPLzEV0bK9DN8Cpvfcna3Hv1h0rkAgUD+hBN+VicAdn6UBSdLdDVIcBiF2xe0
r2hJxFza5/XrFdR99Hv7eEgpMYt0ZRI3XiWh+1DKlvFE16LikXP8I4cGu0FDmNi/
pQyNiD5ipzq2K2zQ/XWjuYszpgXtaaOeUCfM41zey0FldQurMKMLovqCncunRa7S
qwGrjluCuJ8d8UfQAFPGBmVJjEFICkGUl5T2mO5+PjVpr4U8/CUsv0G0UJmbslMD
dQYCYEJ5h27LRDGjO4KapkrepOVQuugECEnJXpVrfN/8VziLeeHQm4joQ/vIa1OJ
FBPs5EQ6200jJ66oUdwC4KKsKZqzmaXW0yllw9dQHCK4WhiRinLVXx+Jol1RpjFm
doCvtEuIpZIUraAc2YsitMBTzZnA1CS+IBtGBA==
=a9Uy
-----END PGP MESSAGE-----
 */

#include <coinsy/stack_impl.hpp>
#include <coinsy/trollbox.hpp>
#include <coinsy/trollbox_manager.hpp>

using namespace coinsy;

trollbox_manager::trollbox_manager(
    boost::asio::io_service & ios, stack_impl & owner
    )
    : io_service_(ios)
    , strand_(ios)
    , stack_impl_(owner)
{
    // ...
}

void trollbox_manager::start()
{
    // ...
}

void trollbox_manager::stop()
{
    for (auto & i : m_trollboxes)
    {
        if (i.second)
        {
            i.second->stop();
        }
    }
}

void trollbox_manager::troll(
    const std::string & key, const std::string & message
    )
{
    auto it = m_trollboxes.find(key);
    
    if (it == m_trollboxes.end())
    {
        auto t = std::make_shared<trollbox>(io_service_, stack_impl_, key);
        
        m_trollboxes[key] = t;
        
        t->start();
        
        t->troll(stack_impl_.username(), message);
    }
    else
    {
        if (it->second)
        {
            it->second->troll(stack_impl_.username(), message);
        }
    }
}

void trollbox_manager::on_find(database::query & q)
{
    if (q.pairs().find("troll") != q.pairs().end())
    {
        auto market = q.pairs()["troll"];
        
        auto it = m_trollboxes.find(market);
        
        if (it != m_trollboxes.end())
        {
            it->second->on_find_troll(q);
        }
    }
}