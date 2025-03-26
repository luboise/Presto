mod  application;

#[macro_export]
macro_rules! EntryPoint {
    ( $( $x:expr ),* ) => {
        {

            let mut temp_vec = Vec::new();
            $(
                temp_vec.push($x);
            )*
            temp_vec
        }
    };
}
/*

// Pulled from somewhere else

    int main(int argc, char** argv) {               
        Presto::Log::init();                        
        PR_CORE_WARN("Initialised core logger.");   
        PR_INFO("Initialised client logger.");      
        AppHandle app{Presto::createApplication()}; 
        app->setup();                               
        app->run();                                 
    };

 PRESTO_APP_CLASS(ClassName)           
    AppHandle Presto::createApplication() {   
        return std::make_unique<ClassName>(); 
    };

// #endif
*/
