#include "keyczar/keyczar.h"

#include "ruby.h"

typedef VALUE (ruby_method)(...);

extern "C" {
  static VALUE m_Keyczar;
  static VALUE c_Signer;
  static VALUE c_Crypter;

  static void keyczar_signer_free(keyczar::Signer *signer){
    delete signer;
  }

  static void keyczar_crypter_free(keyczar::Crypter *crypter){
    delete crypter;
  }

  static VALUE keyczar_signer_alloc(VALUE klass){
    return Data_Wrap_Struct(klass, 0, keyczar_signer_free, 0);
  }

  static VALUE keyczar_crypter_alloc(VALUE klass){
    return Data_Wrap_Struct(klass, 0, keyczar_crypter_free, 0);
  }

  static VALUE keyczar_signer_initialize(VALUE self, VALUE location)
  {
    keyczar::Signer* signer = keyczar::Signer::Read(RSTRING_PTR(location));
    DATA_PTR(self) = signer;
    return self;
  }

  static VALUE keyczar_crypter_initialize(VALUE self, VALUE location)
  {
    keyczar::Crypter* crypter = keyczar::Crypter::Read(RSTRING_PTR(location));
    DATA_PTR(self) = crypter;
    return self;
  }

  static VALUE keyczar_encrypt(VALUE self, VALUE plaintext){
    keyczar::Crypter* crypter;
    Data_Get_Struct(self, keyczar::Crypter, crypter);

    Check_Type(plaintext, T_STRING);
    std::string cryptext=crypter->Encrypt(RSTRING_PTR(plaintext));

    return rb_str_new2(cryptext.c_str());
  }

  static VALUE keyczar_decrypt(VALUE self, VALUE cryptext){
    keyczar::Crypter* crypter;
    Data_Get_Struct(self, keyczar::Crypter, crypter);

    Check_Type(cryptext, T_STRING);
    std::string plaintext=crypter->Decrypt(RSTRING_PTR(cryptext));
    return rb_str_new2(plaintext.c_str());
  }

  static VALUE keyczar_sign(VALUE self, VALUE text){
    keyczar::Signer* signer;
    Data_Get_Struct(self, keyczar::Signer, signer);

    Check_Type(text, T_STRING);
    std::string signature=signer->Sign(RSTRING_PTR(text));

    return rb_str_new2(signature.c_str());
  }

  static VALUE keyczar_verify(VALUE self, VALUE text, VALUE signature){
    keyczar::Signer* signer;
    Data_Get_Struct(self, keyczar::Signer, signer);

    Check_Type(text, T_STRING);
    Check_Type(signature, T_STRING);
    bool result=signer->Verify(RSTRING_PTR(text), RSTRING_PTR(signature));

    return result ? Qtrue : Qfalse;
  }

  void Init_keyczar_ruby() {
    m_Keyczar = rb_define_module("Keyczar");
    c_Signer = rb_define_class_under(m_Keyczar, "Signer", rb_cObject);
    c_Crypter = rb_define_class_under(m_Keyczar, "Crypter", rb_cObject);

    rb_define_method(c_Crypter, "encrypt", (ruby_method*) &keyczar_encrypt, 1);
    rb_define_method(c_Crypter, "decrypt", (ruby_method*) &keyczar_decrypt, 1);

    rb_define_method(c_Signer, "sign", (ruby_method*) &keyczar_sign, 1);
    rb_define_method(c_Signer, "verify", (ruby_method*) &keyczar_verify, 2);

    rb_define_alloc_func(c_Crypter, keyczar_crypter_alloc);
    rb_define_alloc_func(c_Signer, keyczar_signer_alloc);
    rb_define_method(c_Crypter, "initialize", (ruby_method*) &keyczar_crypter_initialize, 1);
    rb_define_method(c_Signer, "initialize", (ruby_method*) &keyczar_signer_initialize, 1);
  }
}
