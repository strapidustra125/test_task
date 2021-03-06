// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: command.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_command_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_command_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_command_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_command_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_command_2eproto;
class ZMQ_cmd;
struct ZMQ_cmdDefaultTypeInternal;
extern ZMQ_cmdDefaultTypeInternal _ZMQ_cmd_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::ZMQ_cmd* Arena::CreateMaybeMessage<::ZMQ_cmd>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class ZMQ_cmd final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:ZMQ_cmd) */ {
 public:
  inline ZMQ_cmd() : ZMQ_cmd(nullptr) {}
  ~ZMQ_cmd() override;
  explicit constexpr ZMQ_cmd(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ZMQ_cmd(const ZMQ_cmd& from);
  ZMQ_cmd(ZMQ_cmd&& from) noexcept
    : ZMQ_cmd() {
    *this = ::std::move(from);
  }

  inline ZMQ_cmd& operator=(const ZMQ_cmd& from) {
    CopyFrom(from);
    return *this;
  }
  inline ZMQ_cmd& operator=(ZMQ_cmd&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const ZMQ_cmd& default_instance() {
    return *internal_default_instance();
  }
  static inline const ZMQ_cmd* internal_default_instance() {
    return reinterpret_cast<const ZMQ_cmd*>(
               &_ZMQ_cmd_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ZMQ_cmd& a, ZMQ_cmd& b) {
    a.Swap(&b);
  }
  inline void Swap(ZMQ_cmd* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ZMQ_cmd* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ZMQ_cmd* New() const final {
    return new ZMQ_cmd();
  }

  ZMQ_cmd* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ZMQ_cmd>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ZMQ_cmd& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const ZMQ_cmd& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ZMQ_cmd* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "ZMQ_cmd";
  }
  protected:
  explicit ZMQ_cmd(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kCommandIDFieldNumber = 1,
    kApplicationIDFieldNumber = 2,
    kCommandDataFieldNumber = 3,
  };
  // uint32 commandID = 1;
  void clear_commandid();
  ::PROTOBUF_NAMESPACE_ID::uint32 commandid() const;
  void set_commandid(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_commandid() const;
  void _internal_set_commandid(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 applicationID = 2;
  void clear_applicationid();
  ::PROTOBUF_NAMESPACE_ID::uint32 applicationid() const;
  void set_applicationid(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_applicationid() const;
  void _internal_set_applicationid(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 commandData = 3;
  void clear_commanddata();
  ::PROTOBUF_NAMESPACE_ID::uint32 commanddata() const;
  void set_commanddata(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_commanddata() const;
  void _internal_set_commanddata(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:ZMQ_cmd)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::uint32 commandid_;
  ::PROTOBUF_NAMESPACE_ID::uint32 applicationid_;
  ::PROTOBUF_NAMESPACE_ID::uint32 commanddata_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_command_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ZMQ_cmd

// uint32 commandID = 1;
inline void ZMQ_cmd::clear_commandid() {
  commandid_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 ZMQ_cmd::_internal_commandid() const {
  return commandid_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 ZMQ_cmd::commandid() const {
  // @@protoc_insertion_point(field_get:ZMQ_cmd.commandID)
  return _internal_commandid();
}
inline void ZMQ_cmd::_internal_set_commandid(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  commandid_ = value;
}
inline void ZMQ_cmd::set_commandid(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_commandid(value);
  // @@protoc_insertion_point(field_set:ZMQ_cmd.commandID)
}

// uint32 applicationID = 2;
inline void ZMQ_cmd::clear_applicationid() {
  applicationid_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 ZMQ_cmd::_internal_applicationid() const {
  return applicationid_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 ZMQ_cmd::applicationid() const {
  // @@protoc_insertion_point(field_get:ZMQ_cmd.applicationID)
  return _internal_applicationid();
}
inline void ZMQ_cmd::_internal_set_applicationid(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  applicationid_ = value;
}
inline void ZMQ_cmd::set_applicationid(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_applicationid(value);
  // @@protoc_insertion_point(field_set:ZMQ_cmd.applicationID)
}

// uint32 commandData = 3;
inline void ZMQ_cmd::clear_commanddata() {
  commanddata_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 ZMQ_cmd::_internal_commanddata() const {
  return commanddata_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 ZMQ_cmd::commanddata() const {
  // @@protoc_insertion_point(field_get:ZMQ_cmd.commandData)
  return _internal_commanddata();
}
inline void ZMQ_cmd::_internal_set_commanddata(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  commanddata_ = value;
}
inline void ZMQ_cmd::set_commanddata(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_commanddata(value);
  // @@protoc_insertion_point(field_set:ZMQ_cmd.commandData)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_command_2eproto
