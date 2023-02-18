#pragma once

#include <stdexcept>
#include <string>
#include <cstdio>


#define BNB_THROW(exception, message)                                                                          \
    do {                                                                                                       \
        const auto er_msg__ = std::string("At ") + __FILE__ + ":" + std::to_string(__LINE__) + ". " + message; \
        std::fprintf(stderr, "BanubaSDK error: %s\n", er_msg__.c_str());                                       \
        throw exception(er_msg__);                                                                             \
    } while (false)

#define BNB_THROW_DOMAIN(exception, domain, message)                                                           \
    do {                                                                                                       \
        const auto er_msg__ = std::string("At ") + __FILE__ + ":" + std::to_string(__LINE__) + ". " + message; \
        std::fprintf(stderr, "BanubaSDK error: %s\n", er_msg__.c_str());                                       \
        throw exception(domain, er_msg__);                                                                     \
    } while (false)

namespace bnb
{
    class domain_specific_data
    {
    public:
        const char* domain() const noexcept
        {
            return m_domain.c_str();
        }
        virtual ~domain_specific_data() = default;

    protected:
        explicit domain_specific_data(std::string domain) noexcept
            : m_domain(std::move(domain))
        {
        }
        std::string m_domain;
    };

    class runtime_error : public std::runtime_error, public domain_specific_data
    {
    public:
        runtime_error(std::string domain, std::string message) noexcept
            : std::runtime_error(message)
            , domain_specific_data(std::move(domain))
        {
        }
    };

    class logic_error : public std::logic_error, public domain_specific_data
    {
    public:
        logic_error(std::string domain, std::string message) noexcept
            : std::logic_error(message)
            , domain_specific_data(std::move(domain))
        {
        }
    };

    class resource_loading_error : public bnb::runtime_error
    {
    public:
        resource_loading_error(
            std::string domain, std::string message, std::string resource
        ) noexcept
            : bnb::runtime_error(std::move(domain), message + ". For: " + resource)
            , m_resource(std::move(resource))
        {
        }

        const char* resource() const noexcept
        {
            return m_resource.c_str();
        }

    protected:
        std::string m_resource;
    };
} // namespace bnb
