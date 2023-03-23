public class HelloNative{
    static{
    System.loadLibrary("HelloNative");  //难点一
    }

    public static native void sayHello();

    public static void main(String[] args){
    new HelloNative().sayHello();
    }
}