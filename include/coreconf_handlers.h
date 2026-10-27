#ifndef CORECONF_HANDLERS_H
#define CORECONF_HANDLERS_H

#include <coap3/coap.h>

#include <cstdint>
#include <vector>

// Macro to create unique namespace per bridge instance
#ifndef CORECONF_PREFIX
#define CORECONF_PREFIX default
#endif

#define _CORECONF_NS_CONCAT(a,b) a##_##b
#define CORECONF_NS_CONCAT(a,b) _CORECONF_NS_CONCAT(a,b)
#define CORECONF_NS CORECONF_NS_CONCAT(coreconf, CORECONF_PREFIX)

namespace CORECONF_NS {

/**
 * Initialize CORECONF request handlers
 * @param ctx The CoAP context to register handlers with
 */
void initialize_handlers(coap_context_t *ctx);

/**
 * Cleanup CORECONF resources
 * Should be called before program exit to free allocated memory
 */
void cleanup_handlers();

/**
 * Get the global coreconfModel pointer
 * Used by implementation code to populate the datastore
 */
void* get_coreconf_model();

/**
 * Rebuild the clookup hashmap after modifying the coreconfModel
 * Call this after manually adding/modifying data in the datastore
 */
void rebuild_clookup_hashmap();

/**
 * Handler for CoAP GET requests
 * Maps to CORECONF GET operation for retrieving single data nodes
 */
void handle_get(coap_resource_t *resource, coap_session_t *session, const coap_pdu_t *request,
                const coap_string_t *query, coap_pdu_t *response);

/**
 * Handler for CoAP FETCH requests (RFC 8132)
 * Maps to CORECONF FETCH operation for retrieving multiple data nodes
 */
void handle_fetch(coap_resource_t *resource, coap_session_t *session, const coap_pdu_t *request,
                  const coap_string_t *query, coap_pdu_t *response);

/**
 * Handler for CoAP PUT requests
 * Maps to CORECONF PUT operation for replacing entire resource
 */
void handle_put(coap_resource_t *resource, coap_session_t *session, const coap_pdu_t *request,
                const coap_string_t *query, coap_pdu_t *response);

/**
 * Handler for CoAP iPATCH requests (RFC 8132)
 * Maps to CORECONF iPATCH operation for partial updates
 */
void handle_ipatch(coap_resource_t *resource, coap_session_t *session, const coap_pdu_t *request,
                   const coap_string_t *query, coap_pdu_t *response);

/**
 * Parse CBOR payload from request
 * @param data Raw CBOR data
 * @param size Size of CBOR data
 * @return true if parsing successful
 */
bool parse_cbor_request(const uint8_t *data, size_t size);

/**
 * Encode CBOR response
 * @param buffer Output buffer for CBOR data
 * @return Size of encoded data
 */
size_t encode_cbor_response(std::vector<uint8_t> &buffer);

}  // namespace CORECONF_NS

// Convenience alias to restore 'coreconf' name within this bridge's code
namespace coreconf = CORECONF_NS;

// Forward-declare all known bridge namespaces for main controller use
namespace coreconf_adc_bridge {}
namespace coreconf_uart_bridge {}

#endif  // CORECONF_HANDLERS_H
