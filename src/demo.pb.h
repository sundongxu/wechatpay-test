// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: demo.proto

#ifndef PROTOBUF_INCLUDED_demo_2eproto
#define PROTOBUF_INCLUDED_demo_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_demo_2eproto 

namespace protobuf_demo_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_demo_2eproto
namespace demo {
class Request;
class RequestDefaultTypeInternal;
extern RequestDefaultTypeInternal _Request_default_instance_;
class Response;
class ResponseDefaultTypeInternal;
extern ResponseDefaultTypeInternal _Response_default_instance_;
}  // namespace demo
namespace google {
namespace protobuf {
template<> ::demo::Request* Arena::CreateMaybeMessage<::demo::Request>(Arena*);
template<> ::demo::Response* Arena::CreateMaybeMessage<::demo::Response>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace demo {

// ===================================================================

class Request : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:demo.Request) */ {
 public:
  Request();
  virtual ~Request();

  Request(const Request& from);

  inline Request& operator=(const Request& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Request(Request&& from) noexcept
    : Request() {
    *this = ::std::move(from);
  }

  inline Request& operator=(Request&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Request& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Request* internal_default_instance() {
    return reinterpret_cast<const Request*>(
               &_Request_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Request* other);
  friend void swap(Request& a, Request& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Request* New() const final {
    return CreateMaybeMessage<Request>(NULL);
  }

  Request* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Request>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Request& from);
  void MergeFrom(const Request& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Request* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string username = 1;
  void clear_username();
  static const int kUsernameFieldNumber = 1;
  const ::std::string& username() const;
  void set_username(const ::std::string& value);
  #if LANG_CXX11
  void set_username(::std::string&& value);
  #endif
  void set_username(const char* value);
  void set_username(const char* value, size_t size);
  ::std::string* mutable_username();
  ::std::string* release_username();
  void set_allocated_username(::std::string* username);

  // string password = 2;
  void clear_password();
  static const int kPasswordFieldNumber = 2;
  const ::std::string& password() const;
  void set_password(const ::std::string& value);
  #if LANG_CXX11
  void set_password(::std::string&& value);
  #endif
  void set_password(const char* value);
  void set_password(const char* value, size_t size);
  ::std::string* mutable_password();
  ::std::string* release_password();
  void set_allocated_password(::std::string* password);

  // string device_id = 3;
  void clear_device_id();
  static const int kDeviceIdFieldNumber = 3;
  const ::std::string& device_id() const;
  void set_device_id(const ::std::string& value);
  #if LANG_CXX11
  void set_device_id(::std::string&& value);
  #endif
  void set_device_id(const char* value);
  void set_device_id(const char* value, size_t size);
  ::std::string* mutable_device_id();
  ::std::string* release_device_id();
  void set_allocated_device_id(::std::string* device_id);

  // string echo_content = 4;
  void clear_echo_content();
  static const int kEchoContentFieldNumber = 4;
  const ::std::string& echo_content() const;
  void set_echo_content(const ::std::string& value);
  #if LANG_CXX11
  void set_echo_content(::std::string&& value);
  #endif
  void set_echo_content(const char* value);
  void set_echo_content(const char* value, size_t size);
  ::std::string* mutable_echo_content();
  ::std::string* release_echo_content();
  void set_allocated_echo_content(::std::string* echo_content);

  // @@protoc_insertion_point(class_scope:demo.Request)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr username_;
  ::google::protobuf::internal::ArenaStringPtr password_;
  ::google::protobuf::internal::ArenaStringPtr device_id_;
  ::google::protobuf::internal::ArenaStringPtr echo_content_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_demo_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Response : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:demo.Response) */ {
 public:
  Response();
  virtual ~Response();

  Response(const Response& from);

  inline Response& operator=(const Response& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Response(Response&& from) noexcept
    : Response() {
    *this = ::std::move(from);
  }

  inline Response& operator=(Response&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Response& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Response* internal_default_instance() {
    return reinterpret_cast<const Response*>(
               &_Response_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Response* other);
  friend void swap(Response& a, Response& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Response* New() const final {
    return CreateMaybeMessage<Response>(NULL);
  }

  Response* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Response>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Response& from);
  void MergeFrom(const Response& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Response* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string echo_content = 2;
  void clear_echo_content();
  static const int kEchoContentFieldNumber = 2;
  const ::std::string& echo_content() const;
  void set_echo_content(const ::std::string& value);
  #if LANG_CXX11
  void set_echo_content(::std::string&& value);
  #endif
  void set_echo_content(const char* value);
  void set_echo_content(const char* value, size_t size);
  ::std::string* mutable_echo_content();
  ::std::string* release_echo_content();
  void set_allocated_echo_content(::std::string* echo_content);

  // int32 status_code = 1;
  void clear_status_code();
  static const int kStatusCodeFieldNumber = 1;
  ::google::protobuf::int32 status_code() const;
  void set_status_code(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:demo.Response)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr echo_content_;
  ::google::protobuf::int32 status_code_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_demo_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Request

// string username = 1;
inline void Request::clear_username() {
  username_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Request::username() const {
  // @@protoc_insertion_point(field_get:demo.Request.username)
  return username_.GetNoArena();
}
inline void Request::set_username(const ::std::string& value) {
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:demo.Request.username)
}
#if LANG_CXX11
inline void Request::set_username(::std::string&& value) {
  
  username_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:demo.Request.username)
}
#endif
inline void Request::set_username(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:demo.Request.username)
}
inline void Request::set_username(const char* value, size_t size) {
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:demo.Request.username)
}
inline ::std::string* Request::mutable_username() {
  
  // @@protoc_insertion_point(field_mutable:demo.Request.username)
  return username_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Request::release_username() {
  // @@protoc_insertion_point(field_release:demo.Request.username)
  
  return username_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Request::set_allocated_username(::std::string* username) {
  if (username != NULL) {
    
  } else {
    
  }
  username_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), username);
  // @@protoc_insertion_point(field_set_allocated:demo.Request.username)
}

// string password = 2;
inline void Request::clear_password() {
  password_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Request::password() const {
  // @@protoc_insertion_point(field_get:demo.Request.password)
  return password_.GetNoArena();
}
inline void Request::set_password(const ::std::string& value) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:demo.Request.password)
}
#if LANG_CXX11
inline void Request::set_password(::std::string&& value) {
  
  password_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:demo.Request.password)
}
#endif
inline void Request::set_password(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:demo.Request.password)
}
inline void Request::set_password(const char* value, size_t size) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:demo.Request.password)
}
inline ::std::string* Request::mutable_password() {
  
  // @@protoc_insertion_point(field_mutable:demo.Request.password)
  return password_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Request::release_password() {
  // @@protoc_insertion_point(field_release:demo.Request.password)
  
  return password_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Request::set_allocated_password(::std::string* password) {
  if (password != NULL) {
    
  } else {
    
  }
  password_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), password);
  // @@protoc_insertion_point(field_set_allocated:demo.Request.password)
}

// string device_id = 3;
inline void Request::clear_device_id() {
  device_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Request::device_id() const {
  // @@protoc_insertion_point(field_get:demo.Request.device_id)
  return device_id_.GetNoArena();
}
inline void Request::set_device_id(const ::std::string& value) {
  
  device_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:demo.Request.device_id)
}
#if LANG_CXX11
inline void Request::set_device_id(::std::string&& value) {
  
  device_id_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:demo.Request.device_id)
}
#endif
inline void Request::set_device_id(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  device_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:demo.Request.device_id)
}
inline void Request::set_device_id(const char* value, size_t size) {
  
  device_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:demo.Request.device_id)
}
inline ::std::string* Request::mutable_device_id() {
  
  // @@protoc_insertion_point(field_mutable:demo.Request.device_id)
  return device_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Request::release_device_id() {
  // @@protoc_insertion_point(field_release:demo.Request.device_id)
  
  return device_id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Request::set_allocated_device_id(::std::string* device_id) {
  if (device_id != NULL) {
    
  } else {
    
  }
  device_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), device_id);
  // @@protoc_insertion_point(field_set_allocated:demo.Request.device_id)
}

// string echo_content = 4;
inline void Request::clear_echo_content() {
  echo_content_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Request::echo_content() const {
  // @@protoc_insertion_point(field_get:demo.Request.echo_content)
  return echo_content_.GetNoArena();
}
inline void Request::set_echo_content(const ::std::string& value) {
  
  echo_content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:demo.Request.echo_content)
}
#if LANG_CXX11
inline void Request::set_echo_content(::std::string&& value) {
  
  echo_content_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:demo.Request.echo_content)
}
#endif
inline void Request::set_echo_content(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  echo_content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:demo.Request.echo_content)
}
inline void Request::set_echo_content(const char* value, size_t size) {
  
  echo_content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:demo.Request.echo_content)
}
inline ::std::string* Request::mutable_echo_content() {
  
  // @@protoc_insertion_point(field_mutable:demo.Request.echo_content)
  return echo_content_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Request::release_echo_content() {
  // @@protoc_insertion_point(field_release:demo.Request.echo_content)
  
  return echo_content_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Request::set_allocated_echo_content(::std::string* echo_content) {
  if (echo_content != NULL) {
    
  } else {
    
  }
  echo_content_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), echo_content);
  // @@protoc_insertion_point(field_set_allocated:demo.Request.echo_content)
}

// -------------------------------------------------------------------

// Response

// int32 status_code = 1;
inline void Response::clear_status_code() {
  status_code_ = 0;
}
inline ::google::protobuf::int32 Response::status_code() const {
  // @@protoc_insertion_point(field_get:demo.Response.status_code)
  return status_code_;
}
inline void Response::set_status_code(::google::protobuf::int32 value) {
  
  status_code_ = value;
  // @@protoc_insertion_point(field_set:demo.Response.status_code)
}

// string echo_content = 2;
inline void Response::clear_echo_content() {
  echo_content_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Response::echo_content() const {
  // @@protoc_insertion_point(field_get:demo.Response.echo_content)
  return echo_content_.GetNoArena();
}
inline void Response::set_echo_content(const ::std::string& value) {
  
  echo_content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:demo.Response.echo_content)
}
#if LANG_CXX11
inline void Response::set_echo_content(::std::string&& value) {
  
  echo_content_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:demo.Response.echo_content)
}
#endif
inline void Response::set_echo_content(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  echo_content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:demo.Response.echo_content)
}
inline void Response::set_echo_content(const char* value, size_t size) {
  
  echo_content_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:demo.Response.echo_content)
}
inline ::std::string* Response::mutable_echo_content() {
  
  // @@protoc_insertion_point(field_mutable:demo.Response.echo_content)
  return echo_content_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Response::release_echo_content() {
  // @@protoc_insertion_point(field_release:demo.Response.echo_content)
  
  return echo_content_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Response::set_allocated_echo_content(::std::string* echo_content) {
  if (echo_content != NULL) {
    
  } else {
    
  }
  echo_content_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), echo_content);
  // @@protoc_insertion_point(field_set_allocated:demo.Response.echo_content)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace demo

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_demo_2eproto
