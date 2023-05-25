import java.io.File;
import java.io.PrintWriter;

public class Example {
    public static final String DUMPSYS_CACHE_PATH = "/home/mi/study/language_study/java/new/diskstats_cache.txt";

    public static void main(String[] args) {
        try {
            File file = new File(DUMPSYS_CACHE_PATH);
            // file.createNewFile();
            PrintWriter pw = new PrintWriter(file);
            // pw.close();
            System.out.println("complete");
        } catch (Exception e) {

        }
    }
}