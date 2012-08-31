#include "keyczar/keyczar.h"

#include "ruby.h"

extern "C" {
  static VALUE Keyczar = Qnil;
  static VALUE Signer = Qnil;
  static VALUE Crypter = Qnil;

  static VALUE keyczar_signer_new(VALUE klass, VALUE location);
  static VALUE keyczar_crypter_new(VALUE klass, VALUE location);

  static VALUE keyczar_encrypt(VALUE self, VALUE plaintext);
  static VALUE keyczar_decrypt(VALUE self, VALUE cryptext);
  static VALUE keyczar_sign(VALUE self, VALUE text);
  static VALUE keyczar_verify(VALUE self, VALUE text, VALUE signature);

  void Init_keyczar_ruby() {
    Keyczar = rb_define_module("Keyczar");
    Signer = rb_define_class_under(Keyczar, "Signer", rb_cObject);
    Crypter = rb_define_class_under(Keyczar, "Crypter", rb_cObject);

    rb_define_method(Crypter, "encrypt", (VALUE (*)(...))keyczar_encrypt, 1);
    rb_define_method(Crypter, "decrypt", (VALUE (*)(...))keyczar_decrypt, 1);

    rb_define_method(Signer, "sign", (VALUE (*)(...))keyczar_sign, 1);
    rb_define_method(Signer, "verify", (VALUE (*)(...))keyczar_verify, 2);

    rb_define_singleton_method(Crypter, "new", (VALUE (*)(...))keyczar_crypter_new, 1);
    rb_define_singleton_method(Signer, "new", (VALUE (*)(...))keyczar_signer_new, 1);
  }

  static void signer_free(keyczar::Signer *ptr){
    delete ptr;
  }

  static void crypter_free(keyczar::Crypter *ptr){
    delete ptr;
  }

  static VALUE keyczar_signer_new(VALUE klass, VALUE location)
  {
    VALUE argv[1];
    keyczar::Signer* ptr = keyczar::Signer::Read(RSTRING_PTR(location));
    VALUE tdata = Data_Wrap_Struct(klass, 0, signer_free, ptr);
    argv[0] = location;
    rb_obj_call_init(tdata, 1, argv);
    return tdata;
  }

  static VALUE keyczar_crypter_new(VALUE klass, VALUE location)
  {
    VALUE argv[1];
    keyczar::Crypter* ptr = keyczar::Crypter::Read(RSTRING_PTR(location));
    VALUE tdata = Data_Wrap_Struct(klass, 0, crypter_free, ptr);
    argv[0] = location;
    rb_obj_call_init(tdata, 1, argv);
    return tdata;
  }

  static VALUE keyczar_encrypt(VALUE self, VALUE plaintext){
    keyczar::Crypter* crypter;
    Data_Get_Struct(self, keyczar::Crypter, crypter);

    std::string cryptext=crypter->Encrypt(RSTRING_PTR(plaintext));

    return rb_str_new2(cryptext.c_str());
  }

  static VALUE keyczar_decrypt(VALUE self, VALUE cryptext){
    keyczar::Crypter* crypter;
    Data_Get_Struct(self, keyczar::Crypter, crypter);

    std::string plaintext=crypter->Decrypt(RSTRING_PTR(cryptext));

    return rb_str_new2(plaintext.c_str());
  }

  static VALUE keyczar_sign(VALUE self, VALUE text){
    keyczar::Signer* signer;
    Data_Get_Struct(self, keyczar::Signer, signer);

    std::string signature=signer->Sign(RSTRING_PTR(text));

    return rb_str_new2(signature.c_str());
  }

  static VALUE keyczar_verify(VALUE self, VALUE text, VALUE signature){
    keyczar::Signer* signer;
    Data_Get_Struct(self, keyczar::Signer, signer);

    bool result=signer->Verify(RSTRING_PTR(text), RSTRING_PTR(signature));

    return result ? Qtrue : Qfalse;
  }
}