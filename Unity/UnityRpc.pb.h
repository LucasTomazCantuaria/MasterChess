// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: UnityRpc.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_UnityRpc_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_UnityRpc_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3015000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3015008 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/empty.pb.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_UnityRpc_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_UnityRpc_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_UnityRpc_2eproto;
::PROTOBUF_NAMESPACE_ID::Metadata descriptor_table_UnityRpc_2eproto_metadata_getter(int index);
namespace UnityRpc {
class PieceMovement;
struct PieceMovementDefaultTypeInternal;
extern PieceMovementDefaultTypeInternal _PieceMovement_default_instance_;
class Vector2Int;
struct Vector2IntDefaultTypeInternal;
extern Vector2IntDefaultTypeInternal _Vector2Int_default_instance_;
}  // namespace UnityRpc
PROTOBUF_NAMESPACE_OPEN
template<> ::UnityRpc::PieceMovement* Arena::CreateMaybeMessage<::UnityRpc::PieceMovement>(Arena*);
template<> ::UnityRpc::Vector2Int* Arena::CreateMaybeMessage<::UnityRpc::Vector2Int>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace UnityRpc {

// ===================================================================

class Vector2Int PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:UnityRpc.Vector2Int) */ {
 public:
  inline Vector2Int() : Vector2Int(nullptr) {}
  virtual ~Vector2Int();
  explicit constexpr Vector2Int(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Vector2Int(const Vector2Int& from);
  Vector2Int(Vector2Int&& from) noexcept
    : Vector2Int() {
    *this = ::std::move(from);
  }

  inline Vector2Int& operator=(const Vector2Int& from) {
    CopyFrom(from);
    return *this;
  }
  inline Vector2Int& operator=(Vector2Int&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Vector2Int& default_instance() {
    return *internal_default_instance();
  }
  static inline const Vector2Int* internal_default_instance() {
    return reinterpret_cast<const Vector2Int*>(
               &_Vector2Int_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Vector2Int& a, Vector2Int& b) {
    a.Swap(&b);
  }
  inline void Swap(Vector2Int* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Vector2Int* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Vector2Int* New() const final {
    return CreateMaybeMessage<Vector2Int>(nullptr);
  }

  Vector2Int* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Vector2Int>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Vector2Int& from);
  void MergeFrom(const Vector2Int& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Vector2Int* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "UnityRpc.Vector2Int";
  }
  protected:
  explicit Vector2Int(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    return ::descriptor_table_UnityRpc_2eproto_metadata_getter(kIndexInFileMessages);
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kXFieldNumber = 1,
    kYFieldNumber = 2,
  };
  // int32 x = 1;
  void clear_x();
  ::PROTOBUF_NAMESPACE_ID::int32 x() const;
  void set_x(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_x() const;
  void _internal_set_x(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 y = 2;
  void clear_y();
  ::PROTOBUF_NAMESPACE_ID::int32 y() const;
  void set_y(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_y() const;
  void _internal_set_y(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:UnityRpc.Vector2Int)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::int32 x_;
  ::PROTOBUF_NAMESPACE_ID::int32 y_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_UnityRpc_2eproto;
};
// -------------------------------------------------------------------

class PieceMovement PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:UnityRpc.PieceMovement) */ {
 public:
  inline PieceMovement() : PieceMovement(nullptr) {}
  virtual ~PieceMovement();
  explicit constexpr PieceMovement(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  PieceMovement(const PieceMovement& from);
  PieceMovement(PieceMovement&& from) noexcept
    : PieceMovement() {
    *this = ::std::move(from);
  }

  inline PieceMovement& operator=(const PieceMovement& from) {
    CopyFrom(from);
    return *this;
  }
  inline PieceMovement& operator=(PieceMovement&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const PieceMovement& default_instance() {
    return *internal_default_instance();
  }
  static inline const PieceMovement* internal_default_instance() {
    return reinterpret_cast<const PieceMovement*>(
               &_PieceMovement_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(PieceMovement& a, PieceMovement& b) {
    a.Swap(&b);
  }
  inline void Swap(PieceMovement* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PieceMovement* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline PieceMovement* New() const final {
    return CreateMaybeMessage<PieceMovement>(nullptr);
  }

  PieceMovement* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<PieceMovement>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const PieceMovement& from);
  void MergeFrom(const PieceMovement& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(PieceMovement* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "UnityRpc.PieceMovement";
  }
  protected:
  explicit PieceMovement(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    return ::descriptor_table_UnityRpc_2eproto_metadata_getter(kIndexInFileMessages);
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStartPosFieldNumber = 1,
    kEndPosFieldNumber = 2,
  };
  // .UnityRpc.Vector2Int startPos = 1;
  bool has_startpos() const;
  private:
  bool _internal_has_startpos() const;
  public:
  void clear_startpos();
  const ::UnityRpc::Vector2Int& startpos() const;
  ::UnityRpc::Vector2Int* release_startpos();
  ::UnityRpc::Vector2Int* mutable_startpos();
  void set_allocated_startpos(::UnityRpc::Vector2Int* startpos);
  private:
  const ::UnityRpc::Vector2Int& _internal_startpos() const;
  ::UnityRpc::Vector2Int* _internal_mutable_startpos();
  public:
  void unsafe_arena_set_allocated_startpos(
      ::UnityRpc::Vector2Int* startpos);
  ::UnityRpc::Vector2Int* unsafe_arena_release_startpos();

  // .UnityRpc.Vector2Int endPos = 2;
  bool has_endpos() const;
  private:
  bool _internal_has_endpos() const;
  public:
  void clear_endpos();
  const ::UnityRpc::Vector2Int& endpos() const;
  ::UnityRpc::Vector2Int* release_endpos();
  ::UnityRpc::Vector2Int* mutable_endpos();
  void set_allocated_endpos(::UnityRpc::Vector2Int* endpos);
  private:
  const ::UnityRpc::Vector2Int& _internal_endpos() const;
  ::UnityRpc::Vector2Int* _internal_mutable_endpos();
  public:
  void unsafe_arena_set_allocated_endpos(
      ::UnityRpc::Vector2Int* endpos);
  ::UnityRpc::Vector2Int* unsafe_arena_release_endpos();

  // @@protoc_insertion_point(class_scope:UnityRpc.PieceMovement)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::UnityRpc::Vector2Int* startpos_;
  ::UnityRpc::Vector2Int* endpos_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_UnityRpc_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Vector2Int

// int32 x = 1;
inline void Vector2Int::clear_x() {
  x_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Vector2Int::_internal_x() const {
  return x_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Vector2Int::x() const {
  // @@protoc_insertion_point(field_get:UnityRpc.Vector2Int.x)
  return _internal_x();
}
inline void Vector2Int::_internal_set_x(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  x_ = value;
}
inline void Vector2Int::set_x(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_x(value);
  // @@protoc_insertion_point(field_set:UnityRpc.Vector2Int.x)
}

// int32 y = 2;
inline void Vector2Int::clear_y() {
  y_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Vector2Int::_internal_y() const {
  return y_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Vector2Int::y() const {
  // @@protoc_insertion_point(field_get:UnityRpc.Vector2Int.y)
  return _internal_y();
}
inline void Vector2Int::_internal_set_y(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  y_ = value;
}
inline void Vector2Int::set_y(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_y(value);
  // @@protoc_insertion_point(field_set:UnityRpc.Vector2Int.y)
}

// -------------------------------------------------------------------

// PieceMovement

// .UnityRpc.Vector2Int startPos = 1;
inline bool PieceMovement::_internal_has_startpos() const {
  return this != internal_default_instance() && startpos_ != nullptr;
}
inline bool PieceMovement::has_startpos() const {
  return _internal_has_startpos();
}
inline void PieceMovement::clear_startpos() {
  if (GetArena() == nullptr && startpos_ != nullptr) {
    delete startpos_;
  }
  startpos_ = nullptr;
}
inline const ::UnityRpc::Vector2Int& PieceMovement::_internal_startpos() const {
  const ::UnityRpc::Vector2Int* p = startpos_;
  return p != nullptr ? *p : reinterpret_cast<const ::UnityRpc::Vector2Int&>(
      ::UnityRpc::_Vector2Int_default_instance_);
}
inline const ::UnityRpc::Vector2Int& PieceMovement::startpos() const {
  // @@protoc_insertion_point(field_get:UnityRpc.PieceMovement.startPos)
  return _internal_startpos();
}
inline void PieceMovement::unsafe_arena_set_allocated_startpos(
    ::UnityRpc::Vector2Int* startpos) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(startpos_);
  }
  startpos_ = startpos;
  if (startpos) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:UnityRpc.PieceMovement.startPos)
}
inline ::UnityRpc::Vector2Int* PieceMovement::release_startpos() {
  
  ::UnityRpc::Vector2Int* temp = startpos_;
  startpos_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::UnityRpc::Vector2Int* PieceMovement::unsafe_arena_release_startpos() {
  // @@protoc_insertion_point(field_release:UnityRpc.PieceMovement.startPos)
  
  ::UnityRpc::Vector2Int* temp = startpos_;
  startpos_ = nullptr;
  return temp;
}
inline ::UnityRpc::Vector2Int* PieceMovement::_internal_mutable_startpos() {
  
  if (startpos_ == nullptr) {
    auto* p = CreateMaybeMessage<::UnityRpc::Vector2Int>(GetArena());
    startpos_ = p;
  }
  return startpos_;
}
inline ::UnityRpc::Vector2Int* PieceMovement::mutable_startpos() {
  // @@protoc_insertion_point(field_mutable:UnityRpc.PieceMovement.startPos)
  return _internal_mutable_startpos();
}
inline void PieceMovement::set_allocated_startpos(::UnityRpc::Vector2Int* startpos) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete startpos_;
  }
  if (startpos) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(startpos);
    if (message_arena != submessage_arena) {
      startpos = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, startpos, submessage_arena);
    }
    
  } else {
    
  }
  startpos_ = startpos;
  // @@protoc_insertion_point(field_set_allocated:UnityRpc.PieceMovement.startPos)
}

// .UnityRpc.Vector2Int endPos = 2;
inline bool PieceMovement::_internal_has_endpos() const {
  return this != internal_default_instance() && endpos_ != nullptr;
}
inline bool PieceMovement::has_endpos() const {
  return _internal_has_endpos();
}
inline void PieceMovement::clear_endpos() {
  if (GetArena() == nullptr && endpos_ != nullptr) {
    delete endpos_;
  }
  endpos_ = nullptr;
}
inline const ::UnityRpc::Vector2Int& PieceMovement::_internal_endpos() const {
  const ::UnityRpc::Vector2Int* p = endpos_;
  return p != nullptr ? *p : reinterpret_cast<const ::UnityRpc::Vector2Int&>(
      ::UnityRpc::_Vector2Int_default_instance_);
}
inline const ::UnityRpc::Vector2Int& PieceMovement::endpos() const {
  // @@protoc_insertion_point(field_get:UnityRpc.PieceMovement.endPos)
  return _internal_endpos();
}
inline void PieceMovement::unsafe_arena_set_allocated_endpos(
    ::UnityRpc::Vector2Int* endpos) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(endpos_);
  }
  endpos_ = endpos;
  if (endpos) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:UnityRpc.PieceMovement.endPos)
}
inline ::UnityRpc::Vector2Int* PieceMovement::release_endpos() {
  
  ::UnityRpc::Vector2Int* temp = endpos_;
  endpos_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::UnityRpc::Vector2Int* PieceMovement::unsafe_arena_release_endpos() {
  // @@protoc_insertion_point(field_release:UnityRpc.PieceMovement.endPos)
  
  ::UnityRpc::Vector2Int* temp = endpos_;
  endpos_ = nullptr;
  return temp;
}
inline ::UnityRpc::Vector2Int* PieceMovement::_internal_mutable_endpos() {
  
  if (endpos_ == nullptr) {
    auto* p = CreateMaybeMessage<::UnityRpc::Vector2Int>(GetArena());
    endpos_ = p;
  }
  return endpos_;
}
inline ::UnityRpc::Vector2Int* PieceMovement::mutable_endpos() {
  // @@protoc_insertion_point(field_mutable:UnityRpc.PieceMovement.endPos)
  return _internal_mutable_endpos();
}
inline void PieceMovement::set_allocated_endpos(::UnityRpc::Vector2Int* endpos) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete endpos_;
  }
  if (endpos) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(endpos);
    if (message_arena != submessage_arena) {
      endpos = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, endpos, submessage_arena);
    }
    
  } else {
    
  }
  endpos_ = endpos;
  // @@protoc_insertion_point(field_set_allocated:UnityRpc.PieceMovement.endPos)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace UnityRpc

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_UnityRpc_2eproto